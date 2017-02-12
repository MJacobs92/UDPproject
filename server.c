#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>

void sendFileOverTCP(struct sockaddr_storage sender, char *tcpPort, char* fileContents)
{
	int socketConn;
	struct sockaddr_in* servaddr;
	// char buffer[1024];

	bzero(&servaddr,sizeof(servaddr));
	servaddr = (struct sockaddr_in*)&sender;

	servaddr->sin_port = htons(atoi(tcpPort));

	//create socket to listen for connections. 
	socketConn = socket(AF_INET, SOCK_STREAM, 0);

	connect(socketConn,(struct sockaddr *)&servaddr,sizeof(servaddr));

	write(socketConn,fileContents,strlen(fileContents)+1);
	printf("File content sent to client: %s", fileContents);
	


	// inet_ntop(AF_INET,&servaddr,buffer,sizeof(buffer));

	// printf("made it to the function\n");
	// // printf("****** trying to extract ip: %u\n", servaddr.sin_addr.s_addr);
	// printf("****** trying to extract ip: %s\n", buffer);



	// socklen_t peer_addrlen;
	// char host[NI_MAXHOST];

	// getnameinfo((struct sockaddr *)&sender, sizeof(sender), host, sizeof(host), NULL, 0, NI_NUMERICHOST);

	// printf("******source ip of client! %s\n", host);

}

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
	// char *message = malloc(1);
	char message[1024];

	char fileContents[1024];
	long fileSize;

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
					sprintf(message, "%s\n", token);

					//send message to the client
					sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&sender,sendsize);

					// reset msg char array
					memset(message,0,sizeof(&message));
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
					FILE *filePointer;

					// attempt to open file
					filePointer = fopen(token,"r");

					if( filePointer == NULL )
					{
						printf("File: %s was not found\n", token);

						//construct message to send back to the client
						sprintf(message, "NOT FOUND\n");

						//send message to the client
						sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&sender,sendsize);
						printf("Message SENT to client: %s\n", message);

						// reset msg char array
						memset(message,0,sizeof(&message));
						
						break;

					}
					//find end of file
					fseek(filePointer,0L,SEEK_END);
					//count bytes in file
					fileSize = ftell(filePointer);
					printf("file size: %zu\n", fileSize );

					//construct message to send back to the client
					sprintf(message, "OK\n%ld\n", fileSize);
					
					rewind(filePointer);

					memset(fileContents,0,sizeof(&fileContents));
					fread(fileContents, fileSize,1, filePointer);

					//send message to the client
					sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&sender,sendsize);

					// reset msg char array
					memset(message,0,sizeof(&message));

					fclose(filePointer);

				}
				else if(count == 2)
				{
					printf("Made it to count 2.\n");
					sendFileOverTCP(sender,token,fileContents);
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