/****************************
 * Author : Holly Buteau
 * Date : Decemebr 3, 2016
 * Program : otp_enc_d
 * Purpose : encrypting 
 * a line
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
	int buffSize = 5000;
	char keyArr[buffSize];
	char encrypt[buffSize];
	char buffer[buffSize];
	int x;
	int y;
	int val = 1;
	int sockfd;
	int newsockfd;
	int portno;
	pid_t pid;
	int n;
	int eNum;
	int kNum;
	int charCount = 0;
	int keyCount = 0;
	char getChars;
	struct sockaddr_in serv_addr, cli_addr;
	char getKey;
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
	//listening up to 5
	listen(sockfd, 5);
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
			//this will read in the first argument,the file to be encrypted
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
			//fills an array with characters to be encrypted
			fgets(encrypt, buffSize, fp);
			for (x = 0; x < strlen(encrypt); x++)
			{
                		encrypt[x] = toupper(encrypt[x]);
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
			{
				//fills the key array		
				fgets(keyArr, buffSize, fp);
				fclose(fp);
				
			}
				
			if (fp == NULL)
			{
				 exit(1);
			}
	//this is the array with the encrypted code
	char final[strlen(encrypt)];

	//here we check the string to be encrypted against the alphabet
	for (x = 0; x < strlen(encrypt); x++)
	{
		for (y = 0; y < strlen(alphabet); y++)
		{
			if (encrypt[x] == alphabet[y]) //if the letters match, we will note the position
			{
				eNum = y;
			}
		}
	
	//now we check the key string against the alphabet	
	for (y = 0; y < strlen(alphabet); y++)
	{
		if(keyArr[x] == alphabet[y])
		{
			kNum = y; //if the letters match, we note the position
		}
	}

	getChars = (eNum + kNum) % 27;//as per the otp instructions
	if (getChars > 27)
	{
		getChars -= 27;
	}	
		
	final[x] = alphabet[getChars];//get the letter and store it
	}

	final[strlen(encrypt) - 1] = '\0';				
	bzero(buffer, buffSize);
	snprintf(buffer, buffSize, "%s", final);
	//sending the encrypted line back to the server
	n = write(newsockfd, final, strlen(final));
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
