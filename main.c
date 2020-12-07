#include "utils.h"
#include "send.h"
#include "receive.h"

int main(int argc, char *argv[]){

    int pid = getpid();
    int send_ret, sequence, finished = 0;

    if (argc != 2 || (strcmp(argv[0], "./traceroute") != 0) || !(checkIP(argv)))
    {
        printf("Wrong input\n");
        return EXIT_FAILURE;
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "sockfd: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    for (int ttl = 1; ttl < 31; ttl++)
    {
        setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

        printf("%d. ", ttl);

        for (int i = 0; i < 3; i++)
        {
            sequence = 10 * ttl + i;
            send_ret = sendICMP(argv, pid, sequence, sockfd);
            if (send_ret < 0)
            {
                fprintf(stderr, "sendICMP: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }
        }

        if (receivePackets(pid, ttl, argv, sockfd)) finished = 1;
        if (finished) return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;

}