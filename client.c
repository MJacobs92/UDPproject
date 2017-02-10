#include "stdio.h"
#include <string.h>

int main() 
{
	// action that the user wants to perform
   char inputAction;
   //string that the user wants capitalized
   char userString[50]
   
   //keep looping until the user decides to quit the program
   while(inputAction != 'q') 
   {
   	printf("Here are your allowed commands:\n s - to enter string to capitalize.\n t - to get a file\n q - to quit\n Enter your command: ");
   	
   	scanf(" %c",&inputAction);

   	//choose an action to perform based on what the user input
   switch(inputAction) {

   	case 's':

   	printf("Please enter a string to capitalize\n");
   	scanf(" %s",&userString);
   		break;
   	case 't':
   	    printf("You entered t\n");
   		break;
   }

}

   return 0;
}

void capitalizeStr(char *userString)
{
	// create a string that is big enough to hold the message to send to the server 
    char *message = malloc(strlen(userString)+5);
    
    // create the string to send the server.
    strcpy(message, "CAP\n");
    strcat(message, userString);
    strcat(message, "\n");
    printf("%s",message);
}