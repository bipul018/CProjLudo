#include "LudoCells.h"

struct Cell
{
	enum CellType type;
	enum Color col;
	int pos;
};
struct Piece
{
	enum Color col;
	char pos;
	struct Cell* cell;
};


// One for each quarter
// Safe cells at 3,8
// Home cell at 8
// Semi final ,ie, gateway to final cell at 6
// Final cells from 13 to 17
// Rest are normal cells
struct Cell cells[4][3 * 6];
struct Cell hell[4];
struct Cell heaven;
struct Piece pieces[4][4];
char playerAvailable[4];

short isInitialized = 0;
enum Color currPlayer = WHITE;
enum Color ranking[4] = { WHITE ,WHITE ,WHITE ,WHITE };
short gamefinish = 0;
unsigned short latestRoll = 0;
char latestCanDo[4];
char colorToChar(enum Color c)
{
	switch (c)
	{
	case RED:
		return 'R';
	case BLUE:
		return 'B';
	case YELLOW:
		return 'Y';
	case GREEN:
		return 'G';
	default:
		return '\0';
	}
}

short getNoInHeaven(enum Color c)
{
	short count = 0;
	for (int i = 0; i < 4; i++)
		if (pieces[c][i].cell->type == HEAVEN)
			count++;
	return count;
}

//retunrns no of completed
static short arrangeRank()
{
	short complete = 0, fills[WHITE];
	for (int i = 0; i < WHITE; i++)
	{
		fills[i] = getNoInHeaven(i);
		if (fills[i] == 4)
			complete++;
	}
	for (int i = 0; i < WHITE; i++)
	{
		for (int j = 0; j < WHITE-(i+1);j++)
		{
			if (fills[ranking[j]] < fills[ranking[j + 1]])
			{
				enum Color tmp = ranking[j];
				ranking[j] = ranking[j + 1];
				ranking[j + 1] = tmp;
			}
		}
	}
	return complete;
}

static void increasePlayer()
{
	do
	{
		currPlayer += 1;
		currPlayer %= WHITE;
	} while (!playerAvailable[currPlayer]);
}

void initNum(int playersNum)
{
	char flags[4];
	for (int i = 0; i < 4; i++)
	{
		if (i < playersNum)
			flags[i] = 1;
		else
			flags[i] = 0;
	}
	initChar(flags);
}

void initChar(char flags[4])
{
	if (isInitialized)
		return;
	srand(time(0));

	heaven.type = HEAVEN; heaven.pos = 100; heaven.col = WHITE;

	for (int i = 0; i < 4; i++)
	{
		ranking[i] = (enum Color)i;
		playerAvailable[i] = flags[i];
		if (currPlayer == WHITE && flags[i])
			currPlayer = i;
		for (int j = 0; j < 4; j++)
		{
			pieces[i][j].col = i; pieces[i][j].cell = &hell[i]; pieces[i][j].pos = j;
			
			hell[i].pos = -100; hell[i].col = i; hell[j].type = HELL;
		}
		for (int j = 0; j < 18; j++)
		{
			if (j > 12)
				cells[i][j].type = FINAL;
			else
				cells[i][j].type = NORMAL;
			cells[i][j].col = i;
			cells[i][j].pos = j;
		}
		cells[i][HELL_GATE].type = SAFE; cells[i][SAFE_GATE].type = SAFE;
	}
	isInitialized = 1;
}

unsigned short rollDice()
{
	//For debug
	latestRoll = rand() % 6 + 1;
	return latestRoll;
}

int choices(int diceVal, char canDo[4])
{
	int freeRoll = (diceVal == 6) || (diceVal == 1);
	int move = 0;

	for (int i = 0; i < 4; i++)
	{
		canDo[i] = 1;
		latestCanDo[i] = 1;
		enum CellType type = pieces[currPlayer][i].cell->type;
		if (type == HELL && freeRoll)
			move++;
		else if (type == NORMAL || type == SAFE)
			move++;
		else if (	type == FINAL && 
					pieces[currPlayer][i].col == pieces[currPlayer][i].cell->col &&
					(HEAVEN_GATE + 1 - pieces[currPlayer][i].cell->pos) >= diceVal)
			move++;
		else
		{
			canDo[i] = 0;
			latestCanDo[i] = 0;
		}
	}
	if (move == 0)
		increasePlayer();
	return move;
}

int isinHeaven(enum Color c, int n)
{
	return pieces[c][n].cell->type == HEAVEN;
}

int isinHell(char* piecechar)
{
	enum Color c;
	switch (*piecechar)
	{
	case 'R':
		c = RED;
		break;
	case 'G':
		c = GREEN;
		break;
	case 'B':
		c = BLUE;
		break;
	case 'Y':
		c = YELLOW;
		break;
	default:
		return -1;
	}
	short n;
	n = atoi(piecechar + 1)-1;
	return pieces[c][n].cell->type == HELL;
}

int move(int num)
{
	int exitFlag = 1;
	if (!latestCanDo[num])
		return-1;

	/*if (latestCanDo[0] != num && latestCanDo[1] != num && latestCanDo[2] != num && latestCanDo[3] != num)
		return -1;*/
	int step = latestRoll;
	enum CellType cellType = pieces[currPlayer][num].cell->type;
	enum Color col = pieces[currPlayer][num].col;
	enum Color cellCol = pieces[currPlayer][num].cell->col;
	int cellPos = pieces[currPlayer][num].cell->pos;

	if (cellType == HELL && (step == 1 || step == 6))
		pieces[currPlayer][num].cell = &cells[col][HELL_GATE];
	else if (col == cellCol && cellType == FINAL && (HEAVEN_GATE + 1 - cellPos) > step)
	{
		pieces[currPlayer][num].cell = &cells[currPlayer][cellPos + step];
	}
	else if (col == cellCol && cellType == FINAL && (HEAVEN_GATE + 1 - cellPos) == step)
	{
		pieces[currPlayer][num].cell = &heaven;
		arrangeRank();
		return 1;
	}
	else if (col == cellCol && cellPos <= FINAL_GATE && (cellPos + step) > FINAL_GATE)
	{
		if (FINAL_GATE + cellPos + step == 18)
		{
			pieces[currPlayer][num].cell = &heaven;
			arrangeRank();
			return 1;
		}
		else
			pieces[currPlayer][num].cell = &cells[currPlayer][FINAL_GATE + cellPos + step];
	}
	else if (cellType == NORMAL || cellType == SAFE)
	{
		struct Cell* dest;
		if (cellPos + step <= 12)
		{
			dest = &cells[cellCol][cellPos + step];
		}
		else
		{
			dest = &cells[(cellCol + 1) % WHITE][cellPos + step - 13];
		}
		if (dest->type == NORMAL)
			for (enum Color i = 0; i < WHITE; i++)
			{
				if (i != col)
					for (int j = 0; j < 4; j++)
					{
						if (pieces[i][j].cell == dest)
						{
							pieces[i][j].cell = &hell[i];
							pieces[currPlayer][num].cell = dest;
							return 2;
						}
					}
			}
		pieces[currPlayer][num].cell = dest;
	}
	else
		return -1;
	if (step == 1 || step == 6)
		return 3;
	increasePlayer();
	return 0;
}

int getCellContent(char *cellLetters, char gottis[16][2])
{
	enum Color c;
	switch (*cellLetters)
	{
	case 'r':
		c = RED;
		break;
	case 'g':
		c = GREEN;
		break;
	case 'b':
		c = BLUE;
		break;
	case 'y':
		c = YELLOW;
		break;
	default:
		return -1;
	}
	short n;
	n = atoi(cellLetters + 1);
	struct Cell* cel = &cells[c][n];
	size_t noOfGottis = 0;
	for (enum Color i = 0; i < WHITE; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (pieces[i][j].cell == cel)
			{
				gottis[noOfGottis][0] = colorToChar(i);
				gottis[noOfGottis][1] = j + '1';
				noOfGottis++;
			}
		}
	}
	return noOfGottis;
}

enum Color getCurrColor()
{
	return currPlayer;
}

enum Color getRank(int i)
{
	return ranking[i];
}

const char* colorToWord(enum Color c)
{
	switch (c)
	{
	case RED:
		return "Red";
	case BLUE:
		return "Blue";
	case YELLOW:
		return "Yellow";
	case GREEN:
		return "GREEN";
	case WHITE:
		return "White";
	default:
		return "Invalid";
	}
}
