#include "utils.h"

int checkIP(char *argv[])
{
    struct sockaddr_in recipient;
	bzero (&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	int ip = inet_pton(AF_INET, argv[1], &recipient.sin_addr);
    return ip;
}

struct icmphdr requestICMP(int pid, int num)
{
    struct icmphdr icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.un.echo.id = pid;
    icmp_header.un.echo.sequence = num;
    icmp_header.checksum = 0;
    icmp_header.checksum = compute_icmp_checksum ((u_int16_t*)&icmp_header, sizeof(icmp_header));
    return icmp_header;
}

u_int16_t compute_icmp_checksum (const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2) sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}