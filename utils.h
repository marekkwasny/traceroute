#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

int checkIP(char *argv[]);
struct icmphdr requestICMP(int pid, int num);
u_int16_t compute_icmp_checksum (const void *buff, int length);