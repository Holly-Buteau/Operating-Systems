/***************************
 * Author : Holly Buteau
 * Date : December 3, 2016
 * Program : keygen
 * Purpose : generate a key
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
  
int main(int argc, char *argv[])
{
	//the array that will be called to generate the alphabet 
        char alphabet[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
         'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};

	//calling random
         srand(time(0));
         int length;
         int x;

         if (argc < 2)
         {
                 printf("Error: Incorrect number of arguments.");
                 exit(1);
         }

         //with help from http://www.thegeekstuff.com/2013/01/c-argc-argv
         length = atoi(argv[1]);
         char newArray[length];
	//fill array
         for (x = 0; x < length; x++)
         {
                 int numLetter;
                 numLetter = rand() % 27;
                 newArray[x] = alphabet[numLetter];
         }
         newArray[length] = '\n';
	//print array
	 for (x = 0; x < length + 1; x++)
         {
                 printf("%c", newArray[x]);

         }

	return 0;
}
