/*********************************************
 * Author : Holly Buteau
 * Title : Program 3 - smallsh
 * Date : November 17, 2016
 * Description : a small shell
*********************************************/

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#define  MAX_LEN  2048
#define  MAX_ARGS  512


int changeDir(char **input);
int exitShell(pid_t *children);
int parseDirections(char *word, char **input);
void progLoop();
void background(pid_t *children);
int octothorpe();
int allOthers(char **input, pid_t *children, int *last, int count);
int statusShell(int *last);
int fileManip(char **input, int flag, int bckgrnd);

int main()
{

         //taken from signal lecture, slide 27 and 28
         struct sigaction act;
         act.sa_flags = 0;

         act.sa_handler = SIG_IGN;
         sigaction(SIGINT, &act, NULL);

	progLoop();	
	return 0;
}


/*************************
 * Fuction Name : progLoop
 * Purpose : run the loop
 * of the shell
**************************/

//I really struggled to get user input, this source was helpful: http://stackoverflow.com/questions/1407872/c-programming-reading-input-and-parsing-it-into-words

void progLoop()
{

	char *line;
	char **args;
	int x;
  	int status = 1;
	int count = 0;
	pid_t *children = malloc(sizeof(pid_t) * 100); //this will hold all of the children
	int *last = malloc(sizeof(int)); //keeps track of the last foreground process
	int pid;


	for (x = 0; x < 100; x++)
	{
		children[x] = 0;	
	}

	
	while (status == 1) 	//while there is valid user input
	{	
		background(children); //verifies that background processes are ended
		
		printf(": ");
		line = malloc(MAX_LEN);
		args = malloc(sizeof(char*) * MAX_ARGS);
		
		fflush(stdout);
		
		fgets(line, MAX_LEN, stdin);
		
		count = parseDirections(line, args); //I struggled for several days with this. this will break up the input. I wasted 4 days trying to figure this out!
	
		//these all match user input with built in directions		
		if (strcmp(args[0], "cd") == 0)
		{
			status = changeDir(args);
						
		}
	
		else if (strcmp(args[0], "exit") == 0)
		{
		
			status = exitShell(children);	
			
		}

		else if (strcmp(args[0], "status") == 0)
		{
			status = statusShell(last);	
		
		}	
	
		//to cancel out the pound sign	
		else if (strcmp(args[0], "#") == 0)
		{
			status = octothorpe();	//becuse the name "hashtag" is stupid. Also octothorp is it's proper name. https://en.wiktionary.org/wiki/octothorpe
	
		}
		
		//takes care of non built in commands				
		else
		{
			status = allOthers(args, children, last, count);
		}
	
		free(line);
		free(args);
	}
	
  		
}

/************************************
 * Function Name : parseDirections
 * Purpose : break apart user input
 * not waste Holly's precious time
************************************/

int parseDirections(char *word, char **input)
{
	int counter = 0;

	
	input[counter] = strtok(word, " \n");

	while(input[counter] != NULL)
	{	
		counter++;
		input[counter] = strtok(NULL, " \n"); //this will place all user input into a very accessible and non time-wasting array.
	}
	
	counter--;//back tracks the last input so the last thing isn't null or newline
	return counter;
}


/******************************
 * Function Name : background
 * Purpose : verify that 
 * background children aren't
 * running
*****************************/

void background(pid_t *children)
{

	int x;
	int stat;
	
	pid_t pid = waitpid(-1, &stat, WNOHANG);
	//this will loop through the array to find fininshed processes and get the pid, exit status, and terminating signal
	
	if (pid > 0)
	{
		for (x = 0; x < 100; x++)
		{
			if (children[x] == pid)
			{
				printf("background pid %d is done: ", pid);
				
				if (WIFEXITED(stat))
					printf("exit value: %d\n", WEXITSTATUS(stat));
			
				else if (WIFSIGNALED(stat))
					printf("terminated by signal %d\n", WTERMSIG(stat));				
				children[x] = 0;
				return;
			}
		}
	}
}
/********************************
 * Function Name : changeDir
 * Purpose : changes directories
********************************/

int changeDir(char **input)
{

	//if the user doesn't specify a dir, we go home
	if (input[1] == NULL)
	{
		//printf("Changing to Home\n");
		char *newDir = getenv("HOME");
		chdir(newDir);
	}
	//go to the user's choice of directories
	else
	{
		//printf("Changing to: %s\n", input[1]);
		char *newDir = input[1];
		chdir(newDir);
	}

	return 1;
}
/***************************************
 * Function Name : exitShell
 * Purpose : free all background processes
*****************************************/

int exitShell(pid_t *children)
{
	int x;
	for (x = 0; x < 100; x++)
	{
		if (children[x] > 0)
			kill(children[x], SIGKILL);
	}
	
	free(children);
		
	return 0;
}

/************************************
 * Function Name : octothorpe
 * Purpose : allow user to make comments
 * and if won't be taken as a command
 * also, to be the superior naming 
 * convention 
 *************************************/

int octothorpe()
{
//	printf("octo called");
	return 1;
}

/********************************
 * Function Name : statusShell
 * Purpose : gets the status of 
 * foreground processes
*********************************/
 
int statusShell(int *last)
{
	int stat;
	stat = *last;
	//normal status ending
	if (WIFEXITED(stat))
		printf("exit value %d\n", WEXITSTATUS(stat));

	//status killed by signal...it may or may not have deserved it. the trial is next month
	else if (WIFSIGNALED(stat))
		printf("terminated by signal %d\n", WTERMSIG(stat));
	
	return 1;
}

/*********************************
 * Function Name : allOthers
 * Purpose : handles all non built
 * in commands
*********************************/

int allOthers(char **input, pid_t *children, int *last, int count)
{

	pid_t pid;
	pid_t wpid;
	int stat;
	int x;
	pid = fork();
	int fileDes;
	int bckgrnd = 0;
	int flag = -1; 
	//verifies if process will run in background
	if (strcmp(input[count], "&") == 0)
	{
		bckgrnd = 1;
		input[count] = NULL;
		count--;
		
		for (x = 0; x < 100; x++)
		{
			if (children[x] == 0)
			{
				children[x] = pid;
			}
		}
	}
	//file manipulation if flag == 1 L it's output
	//if flag == 0, it's input
	//if flag == -1, nothing was chosen
	
	if (input[1] != NULL || bckgrnd == 1)
	{
		if (strcmp(input[1], ">") == 0)
		{
			flag = 1;
		}		
				
		else if (strcmp(input[1], "<") == 0)
		{
			flag = 0;	
		}
		
		fileDes = fileManip(input, flag, bckgrnd);
	}
		
	if (pid == 0)
	{
		if (bckgrnd == 1)
		{
			signal(SIGINT, SIG_IGN);
		}
		
		else
		{
			signal(SIGINT, SIG_DFL);
		}

		if(flag == 1)
		{
			dup2(fileDes, 1);
		}
		
		else if (flag == 0)
		{
			dup2(fileDes, 0);
		}

		if(execvp(input[0], input) == -1)
		{
			printf("Command not recognized");
			exit(1);
		}
	}

	else if (pid < 0)
	{
		printf("Forking Error");
	}


	else
	{
		if (bckgrnd == 0)
		{
			do
			{
				wpid = waitpid(pid, &stat, WUNTRACED);

			}while(!WIFEXITED(stat) && !WIFSIGNALED(stat));
		
			*last = stat;

			if (WIFSIGNALED(stat))
			{
				printf("terminated by signal %d\n", WTERMSIG(stat));
				fflush(stdout);
			}
		}

		else
		{
			printf("background pid is %d\n", pid);
			fflush(stdout);
		}
	}
	
	return 1;
}

/****************************
 * Function Name : fileManip
 * Purpose : opens input and 
 * output files
****************************/
 
int fileManip(char **input, int flag, int bckgrnd)
{
	int fileDes;
	
	if (flag == 1)
	{	//this will open file for writing
		fileDes = open(input[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		//error catching for bad file names
		if (fileDes == -1)
		{
			printf("Error, can not open file\n");
			exit(1);
		}
	
		input[1] = NULL;
		input[2] = NULL;
	}

	else if (flag = 0)
	{	//opens the file for reading
		fileDes = open(input[2], O_RDONLY);
		//error catching for bad file names
		if (fileDes == -1)
		{
			printf("Error, can not open file");
			exit(1);
		}

		input[1] = NULL;
		input[2] = NULL;
	}
	//this takes care of the proces being in the background
	if (bckgrnd == 1)
	{
		if (flag != 1)
			fileDes = open("/dev/null", O_RDONLY);
	
		if (flag != 0)
			fileDes = open("/dev/null", O_WRONLY);
	}

	return fileDes;
}	
	
