#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

int main(int argc, char **argv) 
{
	// read server udp port from command line and store
	int udpPort = atoi(argv[1]);
	printf("udp: %i\n", udpPort);

	char messageBuffer[1024];

	// socket connection
	int socketConn;

	//Struct to hold server connection information
	struct sockaddr_in servaddr;
	struct sockaddr_storage sender;
	socklen_t sendsize = sizeof(sender);

	//ensure servaddr is clear
	bzero( &servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(udpPort);

	//create socket to listen for connections. 
	socketConn = socket(AF_INET, SOCK_DGRAM, 0);

    //Prepare to listen for connections from address/port specified - any ip address and from port specified as commandline argument.
	bind(socketConn, (struct sockaddr *) &servaddr, sizeof(servaddr));

	char *token;
	int count = 0;
	char *message = malloc(1);

	while(1){

    	//clear previous client connection info
		bzero(&sender, sizeof(sender));

    	// read message from client.
		int messageLength = recvfrom(socketConn, messageBuffer, sizeof(messageBuffer), 0, (struct sockaddr*)&sender, &sendsize);

		printf("Message RECEIVED from client: %s\n", messageBuffer);


    	// get the first token which should be the action to perform (ie CAP or FILE)
		token = strtok(messageBuffer, "\n");

		if(strcmp(token,"CAP") == 0)
		{

	   	   // loop through the other tokens 
			while( token != NULL ) 
			{
				if(count == 1)
				{
					for(int i=0;i<strlen(token);i++)
					{
						token[i] = toupper(token[i]);

					}
					printf("Message SENT to client: %s\n", token);

					//construct message to send back to the client
					message = malloc(strlen(token)+5);
					strcpy(message, token);
					strcat(message, "\n");

					//send message to the client
					sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&sender,sendsize);

					// reset msg char array
					memset( message , 0 , sizeof( &message ) ) ;

					//deallocate memory
					free(message);

				}

				token = strtok(NULL, "\n");
				count++;
			}
		}
		else if(strcmp(token,"FILE") == 0)
		{
			while( token != NULL ) 
			{
				if(count == 1)
				{
					FILE *fp;
					fp = fopen(token,"r");

					if( fp == NULL )
					{
						printf("File: %s was not found\n", token);

					//construct message to send back to the client
						char *message = malloc(strlen("NOT FOUND")+5);
						strcpy(message, "NOT FOUND");
						strcat(message, "\n");

					//send message to the client
						sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&sender,sendsize);
						printf("Message SENT to client: %s\n", message);

					// reset msg char array
						memset( message , 0 , sizeof( &message ) ) ;

					//deallocate memory
						free(message);

					}

				}
				else if(count == 2)
				{
					printf("Shouldnt be in here at the moment.\n");
				}

				token = strtok(NULL, "\n");
				count++;
			}

		}
		else{
			printf("Bad Content\n");
		}

		count = 0;

	}

	return 0;

}