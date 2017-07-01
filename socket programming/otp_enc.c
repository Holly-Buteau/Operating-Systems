/************************
 * Author : Holly Buteau
 * Date : December 3, 2016
 * Program : otp_enc
 *************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//calls the error message
void error(const char *msg)
{
	perror(msg); 
	exit(1);
}	

int main(int argc, char *argv[])
{
	//socket,server, and struct code comes from Brewsters lectures	
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int buffSize = 5000;
	char keyArr[buffSize];
	char EncryptArr[buffSize];
	char buffer[buffSize];
	char getChars;
	char getKey;
	int x;
	int charCount = 0;
	int keyCount = 0;
	int sockfd;
	int portno;
	int send; 
	int retrieve; 
	int val = 1;
	int n;
	int size;
	//validating input
	if (argc != 4)
	{
		printf("Error: USAGE: %s hostname port\n", argv[0]);
		exit(1);
	}
	
	//making sockets	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) 
	{
		printf("ERROR opening socket");
		exit(1);
	}
	//this allows us to reuse sockets
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
	//filling struct
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[3]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//connects server and client
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
	{
		printf("ERROR connecting to port %d\n", portno);
		exit(2);
	}
	//checks bad input, pulls the encryption file, which is the first argument sent
	snprintf(buffer, buffSize, "%s", argv[1]);
	for (x = 0; x < strlen(buffer) - 1; x++)
	{
		if(!(isalpha(buffer[x])))
		{
			if (buffer[x] != 32)
			{
				printf("error: input contains bad characters.");
				exit(1);
			}
		}
	}
	//opens up the encryption file and writes it to an array
	FILE *fp = fopen(argv[1], "r"); 
	if (fp)
	{
	
		fgets(EncryptArr, buffSize, fp);
		size = strlen(EncryptArr);
		EncryptArr[size - 1] = '\0';
		fclose(fp);	
	
	}
	//checks for bad files
	else
        {
                error("ERROR: ");
                exit(1);
        }
	//checks bad input
	for (x = 0; x < strlen(EncryptArr) - 1; x++)
	{
		if(!isalpha(EncryptArr[x]))
		{
			if(EncryptArr[x] != 32)
			{
				error("ERROR : input contains bad characters");
				exit(1);
			}
		}
	}
	//pulls the key file, which is the second argument sent
	fp = fopen(argv[2], "r");
	if (fp)
	{
		fgets(keyArr, buffSize, fp);	
		size = strlen(keyArr);
		keyArr[size - 1] = '\0';
		fclose(fp);
        }
	
	
       	
	if (strlen(keyArr) < strlen(EncryptArr))
	{
		printf("Error: key is too short\n");
		exit(1);
	}
	//writing encryption to the client
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0) 
	{
        	error("ERROR writing to socket.\n");
	}

	snprintf(buffer, buffSize, "%s", argv[2]);
	//writing key to the client
	n = write(sockfd, buffer, strlen(buffer));
    	if (n < 0)
	{
        	error("ERROR writing to socket.\n");
	}
	bzero(buffer, buffSize);
	//getting encrypted code back
   	n = read(sockfd, buffer, buffSize);
   	if (n < 0)
	{
        	error("ERROR reading from socket.\n");
	}
	//print encrypted code
	printf("%s\n", buffer);
	close(sockfd);

	return 0;
}	
