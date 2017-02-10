#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) 
{
	// read server udp port from command line and store
	char* udpPort = argv[1];

	printf("udp: %s\n", udpPort);
}