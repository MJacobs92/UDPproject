#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* constructMessageForCap(char *messageString)
{
	// create a string that is big enough to hold the message to send to the server 
	char *message = malloc(strlen(messageString)+5);

    // create the string to send the server.
	strcpy(message, "CAP\n");
	strcat(message, messageString);
	strcat(message, "\n");
	return message;
}

char* constructMessageForReceiveFile(char *messageString, char *tcpPort)
{
	// create a string that is big enough to hold the message to send to the server 
	char *message = malloc(strlen(messageString)+11);

    // create the string to send the server.
	strcpy(message, "FILE\n");
	strcat(message, messageString);
	strcat(message, "\n");
	strcat(message, tcpPort);
	strcat(message, "\n");
}

int main(int argc, char **argv) 
{
	// read tcp port from command  line and store
	char* tcpPort = argv[1];

	// read server ip address from command line and store
	char* serverIp = argv[2];

	// read server udp port from command line and store
	int serverUDPPort = atoi(argv[3]);

	// action that the user wants to perform
	char inputAction;

	//string that the user wants capitalized
	char userString[50];

	//file name entered by user
	char fileName[50];

	// socket connection
	int socketConn;

	//Struct to hold server connection information
	struct sockaddr_in servaddr;

	socklen_t sendsize = sizeof(servaddr);

	//ensure servaddr is clear
	bzero( &servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(serverIp);
	servaddr.sin_port = htons(serverUDPPort);

	//create socket to listen for connections. 
    socketConn = socket(AF_INET, SOCK_DGRAM, 0);

	//keep looping until the user decides to quit the program
	while(inputAction != 'q') 
	{
		printf("Here are your allowed commands:\n s - to enter string to capitalize.\n t - to get a file\n q - to quit\n Enter your command: ");

		fgets(&inputAction,1024,stdin);

   	    //choose an action to perform based on what the user input
		switch(inputAction) {

			case 's':
				printf("Please enter a string to capitalize: ");
				fgets(userString,1024,stdin);
				char* message = constructMessageForCap(userString);
				sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&servaddr,sendsize);
				char receivedMessage[1024];

				while(1)
				{
					recvfrom(socketConn,receivedMessage,1024,0,NULL, NULL);
					if(strcmp(receivedMessage,"") != 0)
					{
						printf("Message from server: %s",receivedMessage);
						break;
					}					
				}
				break;
			case 't':
				printf("Please enter a file name to receive: ");
				fgets(fileName,1024,stdin);
				char* message = constructMessageForReceiveFile(fileName,tcpPort);
				sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&servaddr,sendsize);

				char receivedMessage[1024];

				while(1)
				{
					recvfrom(socketConn,receivedMessage,1024,0,NULL, NULL);
					if(strcmp(receivedMessage,"") != 0)
					{
						printf("Message from server: %s",receivedMessage);
						break;
					}					
				}
				
				break;
		}

	}

	return 0;
}
