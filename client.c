#include "stdio.h"

int main() 
{
   char inputAction;
   
   while(inputAction != 'q') 
   {
   	printf("Here are your allowed commands:\n s - to enter string to capitalize.\n t - to get a file\n q - to quit\n Enter your command: ");
   	
   	scanf(" %c",&inputAction);

   switch(inputAction) {

   	case 's':
   	printf("You entered s\n");
   		break;
   	case 't':
   	    printf("You entered t\n");
   		break;
   }

}

   return 0;
}