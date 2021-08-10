#include <stdio.h>
#include "LudoCells.h"

int main()
{
	short playersN;
	do
	{
		printf("Enter number of players : ");
		scanf(" %u", &playersN);
	} while (playersN < 1);
	scanf("%*c");
	initNum(playersN);
	while (1)
	{
		printBoard();
		printf("%s's turn.\n", colorToWord(getCurrColor()));

		printf("Press enter to roll dice.");
		scanf("%*c");
		short diceval = rollDice();
		printf("Dice shows : %d\n", diceval);
		char pieces[4];
		switch (choices(diceval, pieces))
		{
		case 0:
			printf("Sorry you cannot move.\n");
			break;
		case 1:
		{
			short pos = 0;
			for (int i = 0; i < 4; i++)
			{
				if (pieces[i] == 1)
				{
					pos = i + 1; break;
				}

			}
			printf("Moving piece no %d...\n",pos);
			move(pos - 1);
			break;
		default:
		{
			printf("Pieces you can move are : ");
			for (int i = 0; i < 4; i++)
				if (pieces[i] == 1)
					printf("%d\t", i + 1);

			int pos;
			int moved = 0;
			while (!moved)
			{
				printf("\nChoose among the movable ones: ");
				scanf("%d", &pos);
				for (int i = 0; i < 4; i++)
					if (pieces[i] == 1 && pos == (i + 1))
					{
						move(i);
						printf("Moving piece no %d...\n", pos);
						moved = 1;
					}
			}
		}
		}

		}
		
		if (getNoInHeaven(getCurrColor()) == 4)
		{
			if (getRank(2) == getCurrColor())
			{
				printf("Congratulations, game is completed and the winners are : ");
				printf("First Position : %s\n", colorToWord(getRank(0)));
				printf("Second Position : %s\n", colorToWord(getRank(1)));
				printf("Third Position : %s\n", colorToWord(getRank(2)));
				break;
			}
			else
			{
				printf("%s has completed the game.\n\
Do you wish to continue playing?(Y/N) ", colorToWord(getCurrColor()));
				char cont = 'Y';
				scanf(" %c", &cont);
				if (cont == 'n' || cont == 'N')
					break;
			}
		}

		scanf("%*c");
		system("cls");
	}
	return 0;
}