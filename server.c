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

    while(1){
    	
    	//clear previous client connection info
    	bzero(&sender, sizeof(sender));

    	// read message from client.
    	int messageLength = recvfrom(socketConn, messageBuffer, sizeof(messageBuffer), 0, (struct sockaddr*)&sender, &sendsize);

    	char *token;

    	// get the first token which should be the action to perform (ie CAP or FILE)
   		token = strtok(messageBuffer, "\n");

   		if(strcmp(token,"CAP") == 0)
   		{
   			int count = 0;
	   	   // loop through the other tokens 
		   while( token != NULL ) 
		   {
		      printf( " %s\n", token);
		      printf("%i\n",count );

		      if(count == 1)
		      {
		      	printf("inside count\n");
		      	for(int i=0;i<strlen(token);i++)
		      	{
      				token[i] = toupper(token[i]);
      				
		      	}
		      	printf("%s - final\n", token);

		      }
		    
		      token = strtok(NULL, "\n");
		      count++;
		   }
   		}
   		else if(strcmp(token,"FILE") == 0)
   		{

   		}
   		else{
   			printf("Bad Content\n");
   		}

   		
	}

	return 0;

}