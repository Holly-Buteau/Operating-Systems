/****************************
 * Author : Holly Buteau
 * Date : Decemebr 3, 2016
 * Program : otp_enc_d
 * Purpose : decrypting
 *  a line
 ****************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

//error function
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	//all socket, struct, and client code from Brewster lectures
	//array that holds the alphabet
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
        int x;
       	int buffSize = 5000; 
	char buffer[buffSize];
        char keyArr[buffSize];
        char decrypt[buffSize];
        int y;
	int dNum;
	int kNum;
        int val = 1;
	int sockfd;
        int newsockfd;
        int portno;
        pid_t pid;
        int n;
        char getChars;
        struct sockaddr_in serv_addr, cli_addr;
        socklen_t clilen;
	//validates arguments
	if (argc != 2)
        {
                fprintf(stderr,"Usage: %s port\n", argv[0]);
                exit(1);
        }
	//creates sockets
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if(sockfd < 0)
         {
                printf("Error opening socket\n");
        }
	//filling struct
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
	//binding client and server
        if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        {
                printf("Error on binding\n");
                exit(1);
        }

	//this will allow us to reuse the sockets
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
        listen(sockfd, 5);//listening up to 5
	//while loop for listening
        while (1)
         {
                clilen = sizeof(cli_addr);
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0)
                {
                        printf("Error on accept.\n");
                }

                pid = fork();

                if (pid < 0)
                {
                        printf("Error on fork\n");
                }
	//if fork is succesful, we can begin
                if (pid == 0)
                {
			 bzero(buffer, buffSize);
			//this will read in the first argument,the file to be decrypted
                         n = read(newsockfd, buffer, buffSize);
                         if (n < 0)
                         {
                                 printf("Error reading from socket");
                         }

                         FILE *fp;
                         fp = fopen(buffer, "r");
                         if (fp == NULL)
                         {
                                 exit(1);
                         }
			//fills an array with characters to be decrypted
                         fgets(decrypt, buffSize, fp);
			 for (x = 0; x < strlen(decrypt); x++) 
			 {
        	         	decrypt[x] = toupper(decrypt[x]);
			 }

                         fclose(fp);

                          bzero(buffer, buffSize);
			//this will read in the second argument, the key file
                          n = read(newsockfd, buffer, buffSize);

                         if (n < 0)
                         {
                                 printf("Error opening the key file");
                         }

                         fp = fopen(buffer, "r");
                         if (fp)
                         {	//fills the key array
                          	fgets(keyArr, buffSize, fp);
				fclose(fp);
                         }

			if (fp == NULL)
			{
				exit(1);
			}
			
			
        //this is the array with the encrypted code
	char final[strlen(decrypt)];
	//here we check the string to be decrypted against the alphabet
	for (x = 0; x < strlen(decrypt); x++)
        {
        	 
	       for (y = 0; y < strlen(alphabet); y++)
                {
                        if (decrypt[x] == alphabet[y])//if the letters match, we will note the position
                        {
                       		dNum = y;         
	                }
                }
	//now we check the key string against the alphabet
        for (y = 0; y < strlen(alphabet); y++)
        {
                if(keyArr[x] == alphabet[y])
                {
                      kNum = y;//if the letters match, we note the position
                }
	
	}
	//here we check the decryption and key arrays to decide if they need blank spaces, the 26th character
	if (decrypt[x] == ' ')
	{
		dNum = 26;
	}
	if (keyArr[x] == ' ')
	{
		kNum = 26;
	}
	//as per the otp instructions
        getChars = (dNum - kNum) % 27;
	if (getChars < 0)//here we check to make sure the subtraction didn't go below 0
	{
		getChars += 27;//if so, compensate
	}
        final[x] = getChars + 65;//get the letter and store it
        if (getChars == 26)
	{
		final[x] = ' ';
	}

	}

        final[strlen(decrypt) - 1] = '\0';
       
 
	bzero(buffer, buffSize);
	snprintf(buffer, buffSize, "%s", final);
	//sending the encrypted line back to the server
        n = write(newsockfd, buffer, strlen(buffer));
         if (n < 0)
         {
                 printf("Error writing to socket");
         }

         }

         close(newsockfd);

         }


         close(sockfd);
        


 
          return 0;
}
                                   

