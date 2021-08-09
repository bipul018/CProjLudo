#include <stdio.h>
#include "LudoCells.h"

int main()
{
	initialize();
	while (1)
	{
		printBoard();
		switch (getCurrColor())
		{
		case RED:
			printf("Red's turn.");
			break;
		case BLUE:
			printf("Blue's turn.");
			break;
		case GREEN:
			printf("Green's turn.");
			break;
		case YELLOW:
			printf("Yellow's turn.");
			break;
		}
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
		scanf("%*c");
		system("cls");
	}
	return 0;
}