#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define HELL_GATE 8
#define HEAVEN_GATE 17
#define FINAL_GATE 6
#define SAFE_GATE 3

enum CellType
{
	HELL,
	NORMAL,
	SAFE,
	FINAL,
	HEAVEN
};
enum Color
{
	RED,
	GREEN,
	YELLOW,
	BLUE, 
	WHITE		//Also immovable piece, can be because it has won or is stuck
};

//Initializes the ludo board
void initNum(int playersNum);
void initChar(char flags[4]);

//0,1,2,3,4 for available pieces to move
int choices(int diceVal, char canDo[4]);

//-1 for invalid, 0 for ok done, 1 for ok done and reached heaven 
// 2 for kicking another's to hell, 3 for getting another roll
int move(int num);

unsigned short rollDice();

// Returns no of gottis that are in the queried cell.
int getCellContent(char *cellLetters, char gottis[16][2]);

//Returns if queried piece is in hell.
int isinHell(char* piecechar);

//Prints ludo board
void printBoard();

//Gets current player's color
enum Color getCurrColor();

// Gets corresponding character symbol for color
char colorToChar(enum Color c);

// Gets color value in word form
const char* colorToWord(enum Color c);

//Gets no of pieces already in heaven
short getNoInHeaven(enum Color c);

//Get ranks of heaven
enum Color getRank(int i);

//Returns 1 is is in heaven
int isinHeaven(enum Color c, int n);