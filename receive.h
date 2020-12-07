#include "utils.h"

int receiveICMP(int pid, int ttl, int sockfd);
int receivePackets(int pid, int ttl, char *ip[], int sockfd);