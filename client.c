#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void constructMessageForCap(char message[], char *messageString)
{
	memset(message , 0 , sizeof(&message));
	sprintf(message, "CAP\n%s\n", messageString);
}

void constructMessageForReceiveFile(char message[], char *messageString, char *tcpPort)
{
	memset(message , 0 , sizeof(&message));
	sprintf(message, "FILE\n%s\n%s\n", messageString,tcpPort);
}

char* getUDPMessageFromServer(char *receivedMessage, int socketConn)
{

	while(1)
	{
		recvfrom(socketConn,receivedMessage,1024,0,NULL, NULL);
		if(strcmp(receivedMessage,"") != 0)
		{
			// printf("Message from server: %s",receivedMessage);
			break;
		}					
	}

	return receivedMessage;

}

void receiveFile(char * tcpPort, char* fileName, char* fileSize)
{
	int socketConn;
	int acceptedConnection;
	struct sockaddr_in servaddr;

	char fileContents[atoi(fileSize)];

	// setup tcp socket
	socketConn = socket(AF_INET, SOCK_STREAM, 0);

	bzero( &servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(atoi(tcpPort));

    bind(socketConn, (struct sockaddr *) &servaddr, sizeof(servaddr));

    //listen for tcp connections. up to 10 connections at a time.
    listen(socketConn, 10);

    // accept connections. if there arent any, wait.
    acceptedConnection = accept(socketConn, (struct sockaddr*) NULL, NULL);

    //ensure fileContents is empty.
    bzero(fileContents, sizeof(fileContents));

    while(1)
    {
    	//read data from connection and exit loop once written
    	read(acceptedConnection,fileContents,sizeof(fileContents));
    	if(strcmp(fileContents,"") != 0)
		{
			break;
		}		
    }
    
    //remove \r and \n from file name and write contents to file.
    fileName[strcspn(fileName, "\r\n")] = 0;
    FILE *file = fopen(fileName, "wb");
	fwrite(fileContents, sizeof(char), sizeof(fileContents), file);
	fclose(file);

}

char* getFileSize(char * input)
{
	char *token;
	char *result;
	int count = 0;
	token = strtok(input, "\n");
	while( token != NULL ) 
	{
		if(count == 1)
		{
			result = token;

		}
		token = strtok(NULL, "\n");
		count++;
	}

	return result;
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

	char message[1024];
	char* responseFromServer;

	char receivedMessage[1024];

	//keep looping until the user decides to quit the program
	while(inputAction != 'q') 
	{
		printf("Here are your allowed commands:\n s - to enter string to capitalize.\n t - to get a file\n q - to quit\n Enter your command: ");

		fgets(&inputAction,1024,stdin);		

   	    //choose an action to perform based on what the user input
		switch(inputAction) {

			case 's':
				printf("Please enter a string to capitalize: ");
				
				// get string input from user
				fgets(userString,1024,stdin);

				// create message to send to server
				constructMessageForCap(message,userString);

				//send message to server
				sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&servaddr,sendsize);		

				//get message sent back from server	
				responseFromServer = getUDPMessageFromServer(receivedMessage, socketConn);
				printf("%s",responseFromServer);

				// clear char array to have a clean start for next message.
				memset(receivedMessage , 0 , sizeof(receivedMessage));
				break;
			case 't':
				printf("Please enter a file name to receive: ");

				// get string input from user
				fgets(fileName,1024,stdin);

				// create message to send to server
				constructMessageForReceiveFile(message, fileName,tcpPort);

				//send message to server
				sendto(socketConn,message,strlen(message),0,(struct sockaddr *)&servaddr,sendsize);
				
				//get message sent back from server
				responseFromServer = getUDPMessageFromServer(receivedMessage, socketConn);
				printf("%s",responseFromServer);

				receiveFile(tcpPort,fileName,getFileSize(responseFromServer));

				// clear char array to have a clean start for next message.
				memset(receivedMessage , 0 , sizeof(receivedMessage));

				break;
		}

	}

	return 0;
}
