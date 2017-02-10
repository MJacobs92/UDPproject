#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void capitalizeStr(char *messageString)
{
	// create a string that is big enough to hold the message to send to the server 
	char *message = malloc(strlen(messageString)+5);

    // create the string to send the server.
	strcpy(message, "CAP\n");
	strcat(message, messageString);
	strcat(message, "\n");
	printf("%s",message);
}

void receiveFile(char *messageString)
{
	// create a string that is big enough to hold the message to send to the server 
	char *message = malloc(strlen(messageString)+11);

    // create the string to send the server.
	strcpy(message, "FILE\n");
	strcat(message, messageString);
	strcat(message, "\n");
	strcat(message, tcpPort);
	strcat(message, "\n");
	printf("%s",message);
}

int main(int argc, char **argv) 
{
	// read tcp port from command  line and store
	char* tcpPort = argv[1];

	// read server ip address from command line and store
	char* serverIp = argv[2];

	// read server udp port from command line and store
	char* serverUDPPort = argv[3];

	printf("tcp: %s\n ip: %s\n udp: %s\n", tcpPort, serverIp, serverUDPPort);

	// action that the user wants to perform
	char inputAction;

	//string that the user wants capitalized
	char userString[50];

	//file name entered by user
	char fileName[50];

	//keep looping until the user decides to quit the program
	while(inputAction != 'q') 
	{
		printf("Here are your allowed commands:\n s - to enter string to capitalize.\n t - to get a file\n q - to quit\n Enter your command: ");

		scanf(" %c",&inputAction);

   	    //choose an action to perform based on what the user input
		switch(inputAction) {

			case 's':
			printf("Please enter a string to capitalize: ");
			scanf(" %s",userString);
			capitalizeStr(userString);
			break;
			case 't':
			printf("Please enter a file name to receive: ");
			scanf(" %s",fileName);
			receiveFile(fileName);
			break;
		}

	}

	return 0;
}
