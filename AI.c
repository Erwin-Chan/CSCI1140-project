/**
* CSCI 1140 Programming Laboratory
*
* I declare that the project file here submitted is original
* except for source material explicitly acknowledged,
* and that the same or closely related material has not been
* previously submitted for another course.
* I also acknowledge that I am aware of University policy and
* regulations on honesty in academic work, and of the disciplinary
* guidelines and procedures applicable to breaches of such
* policy and regulations, as contained in the website.
*
* University Guideline on Academic Honesty:
*   http://www.cuhk.edu.hk/policy/academichonesty/
* Faculty of Engineering Guidelines to Academic Honesty:
*   http://www.erg.cuhk.edu.hk/erg-intra/upload/documents/ENGG_Discipline.pdf
*
* Student Name : Chan Siu Chun
* Student ID   : 1155063886
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#define size 30
#define unexplored 7
#define FAKE_EXIT 6
#define MAX_STEP 10 //10:6534, 9:6520

int mazeIdRequest = 7;		// Change this to change the map you use
int seedRequest = 51; //22*, 29*, 40*(total:3/100, turns:6520)
int quickModeRequest = TRUE;   // Change this to turn on/off quick mode

typedef struct New{
	int c;
	int r;
}new;

void clearOutBoundExit(int copyMap[size][size], int fakeExit[size][size]){
	int i, j;
	for(i=0; i<size; i++)
		for(j=0; j<size; j++)
			if(i>19+3 || j>19+3 || i<3 || j<3)
				if(copyMap[i][j]==EXIT){
					fakeExit[i][j]=TRUE;
					copyMap[i][j]=FAKE_EXIT;
				}	
			
	return;
}

void addPair(int rA, int cA, int rB, int cB, new renew[size][size]){
	renew[rB][cB].c = cA;
	renew[rB][cB].r = rA;
}

int isNeighbor(int row, int column, int prevR, int prevC){
	if(row==prevR){
		if(column==prevC-1 || column==prevC+1)
			return TRUE;
		else
			return FALSE;
	}
	else if(column==prevC){
		if(row==prevR-1 || row==prevR+1)
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;

}

int min(int a, int b){
	if(a>b)
		return b;
	else
		return a;

}

void stepTo(int count, int row, int column, int arr[size][size], int copyMap[size][size], new renew[size][size], int step, int myR, int myC){
	
	int outR = renew[row][column].r, outC = renew[row][column].c;
	if(row<0 || row>size || column<0 || column>size || step>MAX_STEP || 
		count >= arr[myR][myC] || copyMap[row][column]==OBSTACLE || copyMap[row][column]==unexplored)
		return;
	else if(myR == row && myC == column){
		count = min(arr[outR][outC], count);
		arr[row][column]=count;			
		return;
	}
	else if(arr[row][column]>=0){
			count = min(arr[outR][outC], count);
			if(outR!=row || outC!=column){
				arr[row][column] = arr[outR][outC];
				return;
			}
			else
				arr[row][column]=count;
			count++;
			step++;
			stepTo(count, row, column+1, arr, copyMap, renew, step, myR, myC); //East
			stepTo(count, row+1, column, arr, copyMap, renew, step, myR, myC); //South
			stepTo(count, row, column-1, arr, copyMap, renew, step, myR, myC); //West
			stepTo(count, row-1, column, arr, copyMap, renew, step, myR, myC); //North
	}
	else
		return;
	
}

void clear(int arr[size][size]){
	int i, j;
	for(i=0; i<size; i++)
		for(j=0; j<size; j++)
			arr[i][j]=100;
}

int countWayOut(int count, int row, int column, int copyMap[size][size], int way, new renew[size][size], int arr[size][size]){
	//way = 2 if find exit
	//way = 7 if find wayout
	row = renew[row][column].r;
	column = renew[row][column].c;
	if(way==EXIT && copyMap[row][column]==unexplored)
		return 100;
	else if(row<0 || row>size || column<0 || column>size || count>12 || copyMap[row][column]==OBSTACLE)
		return 100;
	else if(arr[row][column]<100)
		return count - 100;
	else if(copyMap[row][column]==PASSAGE || copyMap[row][column]==ENTRANCE || copyMap[row][column]==FAKE_EXIT){
			count ++;
			return min(
						min(countWayOut(count, row, column+1, copyMap, way, renew, arr), 
							countWayOut(count, row+1, column, copyMap, way, renew, arr)
							),
						min(countWayOut(count, row, column-1, copyMap, way, renew, arr),
							countWayOut(count, row-1, column, copyMap, way, renew, arr)
							)
						);				
	}
	else if(copyMap[row][column]==way)
		return count;
	else
		return 100;
}



Direction Solve(int column, int row, Tile surroundings[5][5]){

	int i, j;
	//redefine row and column for copyMap
	row = row + 3;
	column = column + 3;


	static int initial = TRUE, term = 0;
	static int copyMap[size][size], fakeExit[size][size], countToExit[size][size], walked[size][size];
	static new renew[size][size];
	static int prevC, prevR;
	static Direction prevDir;
	
	if(GetCurrentTurn()==0){
		initial = TRUE;
		term = 0;
		for(i=0; i<size; i++)
			for(j=0; j<size; j++){
				walked[i][j]=FALSE;
				fakeExit[i][j]=FALSE;
				renew[i][j].r=i;
				renew[i][j].c=j;
			}
	}

	if(initial){ 			//initialize
		for(i=0; i<size; i++)
			for(j=0; j<size; j++){
				copyMap[i][j]=unexplored;
				countToExit[i][j]=100;
			}
		prevC=column;
		prevR=row;
	}

	//copy map
	for(i=-2; i<3; i++){
		for(j=-2; j<3; j++){
				if(copyMap[row+i][column+j]==unexplored)
					copyMap[row+i][column+j]=surroundings[2+i][2+j];
		}
	}

	//find FAKE_ENTRANCE... worm hole
	if(!initial)
		if(!isNeighbor(row, column, prevR, prevC)){
			if(prevDir == EAST){
				if(row == prevR && copyMap[prevR][prevC+1]!=EXIT) //Assume it is Infinity Forest 
					addPair(prevR, prevC, row, column-1, renew); //hence, assume it is two way transition
				addPair(row, column, prevR, prevC+1, renew);
			}
			else if(prevDir == SOUTH){
				if(column == prevC && copyMap[prevR+1][prevC]!=EXIT) //Assume it is Infinity Forest 
					addPair(prevR, prevC, row-1, column, renew); //hence ,assume it is two way transition
				addPair(row, column, prevR+1, prevC, renew);
			}
			else if(prevDir == WEST){
				if(row == prevR && copyMap[prevR][prevC-1]!=EXIT) //Assume it is Infinity Forest 
					addPair(prevR, prevC, row, column+1, renew); //hence ,assume it is two way transition
				addPair(row, column, prevR, prevC-1, renew);
			}
			else if(prevDir == NORTH){
				if(column == prevC && copyMap[prevR-1][prevC]!=EXIT) //Assume it is Infinity Forest 
					addPair(prevR, prevC, row+1, column, renew); //hence, assume it is two way transition
				addPair(row, column, prevR-1, prevC, renew);
			}
		}
	prevR=row;
	prevC=column;


	static int findExit=FALSE;

	for(i=0; i<size; i++)
		for(j=0; j<size; j++){
			if(fakeExit[i][j])
				copyMap[i][j]=FAKE_EXIT;
			if(copyMap[i][j]==EXIT){
				findExit=TRUE;
				countToExit[i][j]=0;
				stepTo(0, i, j, countToExit, copyMap, renew, 0, row, column);
			}
		}


	initial=FALSE;
	////For debug only
	/*
	for(i=0; i<size-5; i++){
		for(j=0; j<size-5; j++){
			int num = countToExit[i][j];
			if(num>99)
				printf("%d ", countToExit[i][j]);
			else if(num>9)
				printf(" %d ", countToExit[i][j]);
			else
				printf("  %d ", countToExit[i][j]);
		}
		printf("\n");
	}
	*/
	/*
	for(i=0; i<size; i++){
		for(j=0; j<size; j++)
			printf("%d ", copyMap[i][j]);
		printf("\n");
	}
	*/
	/////
	if(findExit){
		int E, S, W, N, minimum;
		E=countToExit[row][column+1];
		S=countToExit[row+1][column];
		W=countToExit[row][column-1];
		N=countToExit[row-1][column];
		minimum = min(min(E, S), min(W, N));

		if(minimum>=100)
			findExit = FALSE;
		else{
			if(minimum==E){
				if(E==0){
					clearOutBoundExit(copyMap, fakeExit);
					clear(countToExit);
					fakeExit[row][column+1]=TRUE;
				}
				walked[row][column]=TRUE;
				prevDir = EAST;
				return EAST;
			}
			else if(minimum==S){
				if(S==0){
					clearOutBoundExit(copyMap, fakeExit);
					clear(countToExit);
					fakeExit[row+1][column]=TRUE;
				}
				walked[row][column]=TRUE;
				prevDir = SOUTH;
				return SOUTH;
			}
			else if(minimum==W){
				if(W==0){
					clearOutBoundExit(copyMap, fakeExit);
					clear(countToExit);
					fakeExit[row][column-1]=TRUE;
				}
				walked[row][column]=TRUE;
				prevDir = WEST;
				return WEST;
			}
			else if(minimum==N){
				if(N==0){
					clearOutBoundExit(copyMap, fakeExit);
					clear(countToExit);
					fakeExit[row-1][column]=TRUE;
				}
				walked[row][column]=TRUE;
				prevDir = NORTH;
				return NORTH;
			}
			else
				return 0;
		}
	}

	if(!findExit){
		int E, S, W, N, minimum;
		E=countWayOut(0, row, column+1, copyMap, unexplored, renew, countToExit);
		S=countWayOut(0, row+1, column, copyMap, unexplored, renew, countToExit);
		W=countWayOut(0, row, column-1, copyMap, unexplored, renew, countToExit);
		N=countWayOut(0, row-1, column, copyMap, unexplored, renew, countToExit);
		minimum = min(min(E, S), min(W, N));

		if(minimum==100){
			//1. Find not walked
			//2. Go try obstacle

			//By luck!
			initial = TRUE;
			if(walked[row][column])
				term = (term + 1)%2;
			if(copyMap[row][column+1]!=OBSTACLE){
				walked[row][column]=TRUE;
				return EAST;
			}
			else if(copyMap[row+1][column]!=OBSTACLE){
				walked[row][column]=TRUE;
				return SOUTH;
			}
			else if(copyMap[row][column-1]!=OBSTACLE){
				walked[row][column]=TRUE;
				return WEST;
			}
			else if(copyMap[row-1][column]!=OBSTACLE){
				walked[row][column]=TRUE;			
				return NORTH;
			}
			else
				return 0;
		}
		else if(term == 0){
			if(minimum==E){
				walked[row][column]=TRUE;
				prevDir = EAST;
				return EAST;
			}
			else if(minimum==S){
				walked[row][column]=TRUE;
				prevDir = SOUTH;
				return SOUTH;
			}
			else if(minimum==W){
				walked[row][column]=TRUE;
				prevDir = WEST;
				return WEST;
			}
			else if(minimum==N){
				walked[row][column]=TRUE;
				prevDir = NORTH;
				return NORTH;
			}
			else
				return 0;
		}
		else if(term == 1){		//for area 82 lol
			if(minimum==N){
				walked[row][column]=TRUE;
				prevDir = NORTH;
				return NORTH;
			}
			else if(minimum==W){
				walked[row][column]=TRUE;
				prevDir = WEST;
				return WEST;
			}
			else if(minimum==S){
				walked[row][column]=TRUE;
				prevDir = SOUTH;
				return SOUTH;
			}
			else if(minimum==E){
				walked[row][column]=TRUE;
				prevDir = EAST;
				return EAST;
			}
			else
				return 0;

		}

	}

	return 0;
}



