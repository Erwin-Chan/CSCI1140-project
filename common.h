/*
*  You should not be modifying this file. This file will not be included as part of your
*  project submission. Please put your code into AI.c instead.
*
*
*/

#define _CRT_SECURE_NO_WARNINGS

#define TRUE 1
#define FALSE 0
#define MAZE_DIMENSION 20
#define VISION_DIMENSION 5
#define VISION_RANGE (VISION_DIMENSION-1)/2


typedef enum {NO_MOVEMENT=-1,NORTH=0,EAST=1,SOUTH=2,WEST=3} Direction;
typedef enum {OBSTACLE, PASSAGE, EXIT, ENTRANCE} Tile;
typedef struct {
	int x;   // column
	int y;   // row
} Coordinate;
typedef int bool;

Direction Solve(int positionX, int positionY, Tile surroundings[5][5]);
int GetCurrentTurn();