/*********************************
 * Author : Holly Buteau
 * Title : Program 2 - Adventure
 * Date : October 27, 2016
 * Description : A text-based 
 * adventure game
*********************************/


//Includes
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>

//list of room names
const char *roomName[] =
{
	"Erebor",
	"Hobbiton",
	"Mirkwood",
	"Lothlorien",
	"Rivendell",
	"Mordor",
	"Dale",
	"Isengard",
	"Moria",
	"Gondor" 
};

//list of room types
enum roomType
{
	START_ROOM,
	MID_ROOM,
	END_ROOM
};

//this will contain each room
struct room
{
	enum roomType type;	
	const char *name;
	int currCons;
	int maxCons;
	int position;
	struct room *connectedRooms[6]; //this contains all the rooms each room is connected to
	
};

struct room listOfRooms[7]; //this will hold all the rooms currently in the game
void GameLoop();
struct room *createRooms();
int ranNameGen();
int ranNumGen();
void createFiles(struct room room_list[7], char *directoryName);
void connectRooms(struct room room_list[7]);
bool isConnected(struct room *room1, struct room *room2);



int main()
{
	//this is all setup for creating the directory
	int id = getpid();
	char dirName[30];
	sprintf(dirName, "buteauh.rooms.%d", id);
	mkdir(dirName, 0755);
	//seeding the random number genertor
	srand(time(0));
	createRooms();
	connectRooms(listOfRooms);
	createFiles(listOfRooms, dirName);
	GameLoop();
	return 0;
}

/****************************
 * Function Name : GameLoop
 * Purpose : Runs the game
****************************/

void GameLoop()
{
	int x;
	int y;
	struct room *pastRooms [100];//this will hold the rooms that are visited for the path
	struct room *currRoom = &listOfRooms[0]; //holds the current room the player is in
	int pastIndex = 0;
	int maxRooms = 7; 
	char charBuffer[13]; //for user input
	int steps = 1; //in the instructor's demo, the steps started at 1.
	while (currRoom->type != END_ROOM)
	{	//prints the room info for the user
		printf("\n");
		printf("CURRENT LOCATION: %s\n", currRoom->name);
		printf("POSSIBLE CONNECTIONS: ");
		
		for (x = 0; x < currRoom->currCons; x++)
		{
			if (x < currRoom->currCons - 1)
			{
				printf("%s, ", currRoom->connectedRooms[x]->name);
			}
			
			if (x == currRoom->currCons - 1)
			{
				  printf("%s.\n", currRoom->connectedRooms[x]->name);
			}
		}
		pastRooms[steps - 1] = currRoom; //since we don't know how many steps it will take the user to
		//get to the end, we place rooms at teh step index for looping later
		printf("WHERE TO? >");
		//getting that tricky string input
		fgets(charBuffer, 13, stdin);
		*strchr(charBuffer, '\n') = 0;
		printf("\n");
		bool validInput; //this will validate whether the input is valid
		validInput = false;
		
		for (y = 0; y < currRoom->currCons; y++)
		{

			//compares the user input to all room names	
			if (strncmp(charBuffer, currRoom->connectedRooms[y]->name, 13) == 0)
			{
				currRoom = currRoom->connectedRooms[y];
				steps++; //increments the step counter 
				validInput = true; 
		
			}
			
		
	
		}
		

		if (validInput == false)
		{
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");

		}
	
	}	
	
	
	pastRooms[steps - 1] = currRoom;//since the final room triggers the while loop and doesn't get entered 
	//into the path 
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
	int a;
	for (a = 0; a < steps; a++)
	{ 
		printf("%s", pastRooms[a]->name);
		printf("\n");
	}
	
	return;
	

	
}

/****************************
 * Function Name : createRooms
 * Purpose : Creates the rooms
 * and gives them names, a 
 * type,  and a random number
 * of connections
 ***************************/
	
struct room *createRooms()
{
	
	int storedRoomNums[7];//this will hold the index of the room name to reference
	//from the name list
	int x;
	int y;
	bool roomCheck;//will check if the room name is already taken
	for (x = 0; x < 7; x++)
	{
		 roomCheck = 0;	
			
		do 
		{
			storedRoomNums[x] = ranNameGen();
			
			roomCheck = 0;
 
			for (y = 0; y < 7; y++)
			{
				if ((storedRoomNums[x] == storedRoomNums[y]) && (x != y))
				{
					roomCheck = 1;
					
				}
		
				
								
			}
		
		
			
		}while (roomCheck == 1);

	
	 listOfRooms[x].name = roomName[storedRoomNums[x]];
	 listOfRooms[x].maxCons = rand() % 4 + 3;//random number of connections for each room
	 listOfRooms[x].maxCons += 1;//I kept getting between 2-5 rooms so I gave it a boost
	 listOfRooms[x].currCons = 0;//all rooms start with a random number of connections
	 listOfRooms[x].position = x; 

	//hard codes the room types. The room at 0 will always be the first room, 
	//the room at the end will always be last
		
	if (x == 0)
	{
		listOfRooms[x].type = START_ROOM;
	
	}
	
	else if (x == 6)
	{
		listOfRooms[x].type = END_ROOM;
	
	}

	else
	{
		listOfRooms[x].type = MID_ROOM;

	}
	
	
		
 	}		

	return listOfRooms;//the list of current rooms is created
}

/****************************
 * Function Name : RanNameGen
 * Purpose : Generates a number
 * to reference the room name
 * list
 * ****************************/

int ranNameGen()
{

return	rand() % (11 - 1); 

}

/****************************
 * Function Name : RanNumGen
 * Purpose : generates a random
 * number
****************************/

int ranNumGen()	
{

return rand() % (7 - 1) + 1;

}

/******************************
 * Function Name : CreateFiles
 * Purpose : Creates the Files
******************************/

void createFiles(struct room room_list[7], char *directoryName)
{
	FILE *roomFile;
	int x;
	int y;
	
	chdir(directoryName);//moves directories

	for (x = 0; x < 7; x++)
	{	//this writes all the room info into each room file.
		roomFile = fopen(room_list[x].name, "w");
		fprintf(roomFile, "ROOM NAME: %s\n", room_list[x].name);
		
		for (y = 0; y < room_list[x].currCons; y++)
		{
			
				fprintf(roomFile, "CONNECTION %d : %s\n", y + 1, room_list[x].connectedRooms[y]->name);
		}	

		if (x == 0)
		{
			fprintf(roomFile, "ROOM TYPE: START_ROOM");
		}
		
		else if (x == 6)
		{
			fprintf(roomFile, "ROOM TYPE: END_ROOM");
		}
		
		else
		{
			fprintf(roomFile, "ROOM TYPE: MID_ROOM");
		}
	}

			

	fclose(roomFile);
	chdir("..");//moves back to previous directory
	
}

/******************************
 * Function Name : connectRooms
 * Purpose : Connects the rooms 
 * to each other
********************************/

void connectRooms(struct room room_list[7])
{
	int ranNum;
	int x;
	int y;
	int addRoom;//this will be a random number to test indexes
	int room2;
	bool minReached;
	minReached = false;
  	
	for (x = 0; x < 7; x++)
	{	


		for (y = 0; y < room_list[x].maxCons; y++)//here we cycle through the connected room struct
		//to see if the rooms are already connected
		{
			addRoom = ranNumGen();
		
			if (room_list[x].currCons < room_list[x].maxCons && room_list[addRoom].currCons < room_list[x].maxCons)//making sure both rooms won't exceed their connections
			{ 
				if (!isConnected(&room_list[x], &room_list[addRoom])) //check to see if they're already connected
				{
 						//this will connect both rooms to each other
						room_list[x].connectedRooms[room_list[x].currCons] = &room_list[addRoom];
						room_list[x].currCons++;
						room_list[addRoom].connectedRooms[room_list[addRoom].currCons] = &room_list[x];
						room_list[addRoom].currCons++;		
				}
							
			}
			
		

		
		}	
	}	
}

/*******************************
 * Function Name : isConnected
 * Purpose : checks if two rooms
 * are already connected
*******************************/

bool isConnected(struct room *room1, struct room *room2)
{	
	int x;

	if (room1 == room2)//checks to make sure it's not the same room
	{
		return true;
		
	}
	//checks the connected room struct to see if the room is already there
	for (x = 0; x < room1->currCons; x++)
	{
		if (room1->connectedRooms[x] == room2)
		{
 
			return true;

		}

	}
	
		
	return false;
} 	


						
