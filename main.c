// If you are using Mac, please open
// Terminal->Preferences->Profiles->Advanced
// Select "Russian (DOS)" in the "Text encoding" box

/*
 *  You should not be modifying this file. This file will not be included as part of your
 *  project submission. Please put your code into AI.c instead.
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "hundred.h"

#define TILE_CYCLE 5
typedef enum { ACT_OBSTACLE = 0, ACT_PASSAGE = 1, ACT_EXIT = 2, ACT_ENTRANCE = 3, ABS_BARRIER = TILE_CYCLE, FAKE_EXIT = 2 + TILE_CYCLE, FAKE_ENTRANCE = 1 + TILE_CYCLE} ActualTile;

extern int ultimateMaze[100][MAZE_DIMENSION][MAZE_DIMENSION];
static int _turn;		// for reporting to the maze solver

Coordinate SetupMaze(ActualTile m[MAZE_DIMENSION][MAZE_DIMENSION], int id, int seed, Coordinate *playerLocation) {
	static int autoCounter = 0;
	static int trivialMaze[MAZE_DIMENSION][MAZE_DIMENSION] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	static int easyMaze[MAZE_DIMENSION][MAZE_DIMENSION] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 3, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	static int loopMaze[MAZE_DIMENSION][MAZE_DIMENSION] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 3, 1, 0, 1, 1, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	static int hallMaze[MAZE_DIMENSION][MAZE_DIMENSION] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0,
		0, 1, 1, 2, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	static int infiniteMaze[MAZE_DIMENSION][MAZE_DIMENSION] = {
		0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
		0, 3, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0,
		0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
		0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0,
		1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1,
		0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0,
		0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 2, 0, 1, 1, 1, 1, 0, 1, 0,
		1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
	};
	static int teleportMaze[MAZE_DIMENSION][MAZE_DIMENSION] = {
		0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		1, 1, 0, 0, 7, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1,
		0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 3, 0,
		0, 1, 0, 7, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0,
		0, 1, 0, 0, 0, 1, 0, 1, 6, 1, 0, 0, 0, 1, 1, 1, 1, 0, 7, 0,
		0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
		0, 6, 0, 2, 0, 7, 0, 0, 0, 1, 0, 1, 1, 6, 0, 0, 0, 0, 1, 0,
		0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 6, 1, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		
		
	};
	static Coordinate dispDimension = { MAZE_DIMENSION, MAZE_DIMENSION };

	int i, j;

	
	if (id <= 2) {
		// some small hard-coded mazes...
		for (i = 0; i < MAZE_DIMENSION; i++) {
			for (j = 0; j < MAZE_DIMENSION; j++) {
				if (id == 0) m[i][j] = trivialMaze[i][j];
				else if (id == 1) m[i][j] = easyMaze[i][j];
				else if (id == 2) m[i][j] = loopMaze[i][j];
			}
		}
		dispDimension.x = 11;
		dispDimension.y = 7;
	}
	else if (id <= 5) {
		// large hard-coded mazes...
		for (i = 0; i < MAZE_DIMENSION; i++) {
			for (j = 0; j < MAZE_DIMENSION; j++) {
				if (id == 3) m[i][j] = hallMaze[i][j];
				else if (id == 4) m[i][j] = infiniteMaze[i][j];
				else if (id == 5) m[i][j] = teleportMaze[i][j];
			}
		}
		dispDimension.x = 20;
		dispDimension.y = 10;
	}	
	else {
		// a maze from the one hundred mazes (based on seed )...
		for (i = 0; i < MAZE_DIMENSION; i++) {
			for (j = 0; j < MAZE_DIMENSION; j++) {
				if (id == 6) m[i][j] = ultimateMaze[seed % 100][i][j];
				else if (id == 7) m[i][j] = ultimateMaze[autoCounter][i][j];
			}
		}
		autoCounter++;
		dispDimension.x = MAZE_DIMENSION;
		dispDimension.y = MAZE_DIMENSION;
	}

	// place player at main entrance
	for (i = 0; i < MAZE_DIMENSION; i++) {
		for (j = 0; j < MAZE_DIMENSION; j++) {
			if (m[i][j] == ENTRANCE) {
				playerLocation->x = j;
				playerLocation->y = i;
			}
		}
	}

	return dispDimension;
}

void DisplayMaze(ActualTile m[MAZE_DIMENSION][MAZE_DIMENSION], Coordinate dispDimension, Coordinate playerLocation, bool playerDead) {
	
	int i, j;
	
	for (i = 0; i < dispDimension.y; i++) {
		// display the overhead map...
		printf("               ");
		for (j = 0; j < dispDimension.x; j++) {
			if (i <= playerLocation.y + VISION_RANGE && i >= playerLocation.y - VISION_RANGE
				&& j <= playerLocation.x + VISION_RANGE && j >= playerLocation.x - VISION_RANGE 
				&& !playerDead) {
				if (playerLocation.y == i && playerLocation.x == j)	printf("&");
				else if (m[i][j] % TILE_CYCLE == 0) printf("%c", 178);
				else if (m[i][j] % TILE_CYCLE == 1) printf(".");
				else if (m[i][j] % TILE_CYCLE == 2) printf("E");
				else if (m[i][j] % TILE_CYCLE == 3) printf("S");
			}
			
			else {
				if (playerLocation.y == i && playerLocation.x == j)	printf("&");
				else if (m[i][j] % TILE_CYCLE == 0) printf("%c", 176);
				else if (m[i][j] % TILE_CYCLE == 1) printf(" ");
				else if (m[i][j] % TILE_CYCLE == 2) printf("E");
				else if (m[i][j] % TILE_CYCLE == 3) printf("S");
			}
			
		}
			
		printf("\n");
	}
}

bool MovePlayer(ActualTile m[MAZE_DIMENSION][MAZE_DIMENSION], Coordinate mazeLimits, Coordinate *playerLocation, Direction moveDir) {
	int destX = playerLocation->x;
	int destY = playerLocation->y;
	if (moveDir == NORTH) {
		destY = (playerLocation->y - 1 + mazeLimits.y) % mazeLimits.y;
	} else if (moveDir == SOUTH) {
		destY = (playerLocation->y + 1 + mazeLimits.y) % mazeLimits.y;
	} else if (moveDir == WEST) {
		destX = (playerLocation->x - 1 + mazeLimits.x) % mazeLimits.x;
	} else if (moveDir == EAST) {
		destX = (playerLocation->x + 1 + mazeLimits.x) % mazeLimits.x;
	} else {
		return TRUE;
	}

	if (m[destY][destX] == ACT_OBSTACLE)
		return FALSE;
	else {
		playerLocation->x = destX;
		playerLocation->y = destY;
		if (m[destY][destX] == FAKE_EXIT) {
			// place player at a certain fake entrance
			int i, j;
			srand(destY*1000+destX);
			int iBias = rand() % MAZE_DIMENSION;
			int jBias = rand() % MAZE_DIMENSION;
			for (i = 0; i < MAZE_DIMENSION; i++) {
				for (j = 0; j < MAZE_DIMENSION; j++) {
					int w = (i + iBias) % MAZE_DIMENSION;
					int z = (j + jBias) % MAZE_DIMENSION;
					
					if (m[w][z] == FAKE_ENTRANCE) {
						playerLocation->x = z;
						playerLocation->y = w;
						return TRUE;
					}
				}
			}
		}

		return TRUE;
	}
	
	
}

void ClearScreen() {
#if defined(_WIN32) || defined(_WIN64)
	system("cls");			// Windows-only
#else
	system("clear");
#endif
}

void SetupDisplay() {
#if defined(_WIN32) || defined(_WIN64)
	system("chcp 437");		// Windows-only
#endif
	ClearScreen();
}

char GetKey() {
#if defined(_WIN32) || defined(_WIN64)

	return getch();			// Windows-only
#else
	return getchar();
#endif
}

int GetCurrentTurn() {
	return _turn;
}

int main(void)
{
	extern int mazeIdRequest;			// requested maze Id
	extern int seedRequest;				// requested random seed
	extern int quickModeRequest;		// whether quick mode is requested
	
	ActualTile maze[MAZE_DIMENSION][MAZE_DIMENSION];
	Coordinate playerLocation = { -1, -1 };

	SetupDisplay();
	// GenerateMazes("hundred.h",0,100); // HEHE, you can't do this!

	int mazeToUse = mazeIdRequest;
	int seedToUse = seedRequest;
	bool useQuickMode = quickModeRequest;
	int numMazes = 1;
	if (mazeToUse == 7) {
		numMazes = 100;		// scoring mode...
		useQuickMode = TRUE;
	}

	int totalTurns = 0;
	
	int metaTurn;
	for (metaTurn = 0; metaTurn < numMazes; metaTurn++) {
		// setup maze
		Coordinate dispDimension = SetupMaze(maze, mazeToUse, seedToUse, &playerLocation);
		int maxTurns = (dispDimension.x - 2)*(dispDimension.y - 2);   // approximately twice of walking space...
		
		// main loop
		Direction lastMoveDir = NO_MOVEMENT;
		bool lastMoveSucceed = FALSE;
		bool finishedMaze = FALSE;
		bool timeout = FALSE;
		
		int turn;
		for (turn = 0; turn <= maxTurns; turn++) {
			_turn = turn;
			// check winning/losing condition
			if (maze[playerLocation.y][playerLocation.x] == EXIT) {
				finishedMaze = TRUE;
			}
			else if (turn == maxTurns) {
				timeout = TRUE;    // last turn is an ending turn
			}

			// for every turn, display what we have if not in quick mode or not timeout/finished ...
			// (we also won't display when number of mazes is > 1)
			if (numMazes == 1 && (useQuickMode == FALSE || timeout || finishedMaze)) {
				ClearScreen();
				printf("CSC1140 Fall 2015 - The Dark Forest v.1.0\n\n");
				printf("Maze: #%d Seed: %d - Turn : %d/%d ", mazeIdRequest, seedRequest, turn, maxTurns);
				if (lastMoveDir != NO_MOVEMENT) {
					printf("| You just moved ");
					if (lastMoveDir == NORTH) printf("north ");
					else if (lastMoveDir == EAST) printf("east ");
					else if (lastMoveDir == SOUTH) printf("south ");
					else if (lastMoveDir == WEST) printf("west ");
					if (lastMoveSucceed == FALSE) printf("but failed!\n\n");
					else printf("\n\n");
				}
				else {
					printf("\n\n");
				}
				DisplayMaze(maze, dispDimension, playerLocation, timeout);

			}

			// break way if done
			if (timeout || finishedMaze) {
				break;
			}

			// if a normal turn, we ask for a step...
			static Tile vision[VISION_DIMENSION][VISION_DIMENSION];
			int i, j;
			for (i = 0; i < VISION_DIMENSION; i++) {
				for (j = 0; j < VISION_DIMENSION; j++) {

					int mapX = (playerLocation.x - VISION_RANGE + j + dispDimension.x) % dispDimension.x;
					int mapY = (playerLocation.y - VISION_RANGE + i + dispDimension.y) % dispDimension.y;
					if (mapX >= 0 && mapX < MAZE_DIMENSION
						&& mapY >= 0 && mapY < MAZE_DIMENSION) {
						vision[i][j] = maze[mapY][mapX] % TILE_CYCLE;
					}
				}
			}
			Direction playerMoveDir = Solve(playerLocation.x, playerLocation.y, vision);

			// really move it - but not on display yet...
			lastMoveSucceed = MovePlayer(maze, dispDimension, &playerLocation, playerMoveDir);
			lastMoveDir = playerMoveDir;


			// get key now so player debug messages can be seen
			if (useQuickMode == FALSE) {
				printf("\nPress Enter to continue (ctrl+C to terminate)...\n");
				getchar();
			}



		}
		
		// aftermath for each maze
		totalTurns += turn;
		if (numMazes == 1) {
			// normal single maze mode ending
			if (finishedMaze) {
				printf("\n       ******** And you escaped the maze in %d turns! ******** \n", turn);
				printf("       ******** Press q to quit                       ******** \n");

			}
			else {
				printf("\n       ******** You run out of energy and died in the maze...     ******** \n");
				printf("       ******** You will be missed by your CSCI1140 classmates... ******** \n");
			#if defined(_WIN32) || defined(_WIN64)
				printf("       ******** Press q to quit                                   ******** \n");
			#else
				printf("       ******** Press q then Enter to quit                        ******** \n");
			#endif

			}
			char c;
			do {
				c = GetKey();
			} while (c != 'q');

		}
		else {
			// multi-maze scoring mode
			if (finishedMaze) {
				printf("You escaped area %d in %d turns...\n", metaTurn, turn);
			}
			else {
				printf("You run out of energy in area %d and wake up at exit (%d turns counted)...\n", metaTurn, turn);
			}
		}

	}

	if (numMazes > 1) {
		printf("\n\n    ***                                               ***\n");
		printf("    *** Turns used to escape the Dark Forest: %6d  ***\n", totalTurns);
		printf("    ***                                               ***\n\n");
	}
	
	return 0;
}
