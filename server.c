#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) 
{
	// read server udp port from command line and store
	char* udpPort = argv[1];
	printf("udp: %s\n", udpPort);

	int socket;

	struct sockaddr_in servaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(udpPort);

    socket = socket(PF_INET, SOCK_DGRAM, 0);
}