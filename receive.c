#include "utils.h"

int times[3];
char addresses[3][20];
struct timeval now;
long long start, current;
int avg = 0, avgtmp = 0;

int receiveICMP(int pid, int ttl, int sockfd)
{
    gettimeofday(&now, NULL);
    struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];

    ssize_t packet_len = recvfrom(
		sockfd,
		buffer,
		IP_MAXPACKET,
		MSG_DONTWAIT,
		(struct sockaddr *)&sender,
		&sender_len);

	if (packet_len < 0)
		fprintf(stderr, "recvfrom: %s\n", strerror(errno));

    char ip_str[20];
    int sender_ttl, sender_seq;    

	inet_ntop(AF_INET, &(sender.sin_addr), ip_str, sizeof(ip_str));

    struct iphdr* ip_header = (struct iphdr*) buffer;
    u_int8_t* icmp_packet = buffer + 4 * ip_header->ihl;
    struct icmphdr* icmp_header = (struct icmphdr*) icmp_packet;

    sender_ttl = icmp_header->un.echo.sequence / 10;
    sender_seq = icmp_header->un.echo.sequence % 10;

    if (icmp_header->type == ICMP_ECHOREPLY && icmp_header->un.echo.id == pid && sender_ttl == ttl)
    {
        strcpy(addresses[sender_seq], ip_str);
        current = ((long long)now.tv_sec * 1000) + (now.tv_usec / 1000);
        times[sender_seq] = (int)(current - start);
        avgtmp++;
        return 1;
    }

    if (icmp_header->type == ICMP_TIME_EXCEEDED)
    {
        u_int8_t *ptr = buffer;
        ptr += 4 * ip_header->ihl + 8;
		struct iphdr *original = (struct iphdr *)ptr;
		ptr += (original->ihl * 4);
		struct icmphdr *icmp = (struct icmphdr *)ptr;

        sender_ttl = icmp->un.echo.sequence / 10;
        sender_seq = icmp->un.echo.sequence % 10;

        if (icmp->un.echo.id == pid && sender_ttl == ttl)
        {
            strcpy(addresses[sender_seq], ip_str);
            current = ((long long)now.tv_sec * 1000) + (now.tv_usec / 1000);
            times[sender_seq] = (int)(current - start);
            avgtmp++;
            return 1;
        }

    }
    return 0;

}

int receivePackets(int pid, int ttl, char *ip[], int sockfd)
{
    gettimeofday(&now, NULL);
    start = ((long long)now.tv_sec * 1000) + (now.tv_usec / 1000);
    
    int packets = 0;
    int receive_ret;
    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(sockfd, &descriptors);
    struct timeval tv; tv.tv_sec = 1; tv.tv_usec = 0;

    for (int i = 0; i < 3; i++)
    {
        strcpy(addresses[i], "0");
    }

    while (packets < 3)
    {
        int ready = select(sockfd+1, &descriptors, NULL, NULL, &tv);

        if (ready > 0)
        {
            receive_ret = receiveICMP(pid, ttl, sockfd);
            if (receive_ret) packets++;
        }
        else break;        

    }

    if (!(strcmp(addresses[0], "0") || strcmp(addresses[1], "0") || strcmp(addresses[2], "0")))
    {
        printf("*\n");
        return 0;
    }
    
    int reached = 0;
    for (int i = 0; i < 3; i++)
    {
        avg += times[i];

        int print = 0;
        if (!(strcmp(addresses[i], ip[1])))
            {
                reached = 1;
            }
        if (i > 0)
        {
            for (int j = 0; j < i; j++)
            {
                if (strcmp(addresses[i], addresses[j])) print = 1;
            } 
            if (print)
            {
                printf("%s ", addresses[i]);
                print = 0;
            }
        } 
        if (i == 0)
        { 
            if ((strcmp(addresses[0], "0"))) printf("%s ", addresses[i]);
        }    
    }
    if (avgtmp == 3)
    {
        printf("%d ms\n", avg/avgtmp);
    }
    if (avgtmp < 3 && avgtmp > 0) printf("??? \n");
    if (avgtmp == 0) printf("\n");


    avg = 0;
    avgtmp = 0;
    if (reached) return 1;
    else return 0;
    
}