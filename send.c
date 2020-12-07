#include "utils.h"

int sendICMP(char *argv[], int pid, int num, int sockfd)
{
    struct icmphdr icmp_header = requestICMP(pid, num);

    struct sockaddr_in recipient;
	bzero (&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &recipient.sin_addr);

    ssize_t bytes_sent = sendto(
        sockfd,
        &icmp_header,
        sizeof(icmp_header),
        0,
        (struct sockaddr*)&recipient,
        sizeof(recipient));

    return bytes_sent;
}