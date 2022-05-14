#include "LudoCells.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LudoCells.h"
const char* BOARD_DESIGN = "\n\
____________________________________________________________________________________\n\
|   =============             |......|........|......|           =============     |\n\
|   [           ]             |  b5  |   b6   |  b7  |           [           ]     |\n\
|   [  Y1   Y2  ]             |  b4  | ~ b13  |  b8 *|           [  B1   B2  ]     |\n\
|   [           ]             |______|________|______|           [           ]     |\n\
|   [  Y3   Y4  ]             |* b3  |   b14  |  b9  |           [  B3   B4  ]     |\n\
|   [           ]             |  b2  |   b15  |  b10 |           [           ]     |\n\
|   =============             |______|________|______|           =============     |\n\
|                             |  b1  |   b16  |  b11 |                             |\n\
|                             |  b0  |   b17  |  b12 |                             |\n\
|                             |......|........|......|                             |\n\
|   |.........................|                      |.........................|   |\n\
|   |      *                  |                      |              *          |   |\n\
|   | y7  y8  y9  y10 y11 y12 |     =============    | r0  r1  r2  r3  r4  r5  |   |\n\
|   |_________________________|     [ C1        ]    |_________________________|   |\n\
|   |     ~                   |     [ C2        ]    |                 ~       |   |\n\
|   | y6  y13 y14 y15 y16 y17 |     [           ]    | r17 r16 r15 r14 r13 r6  |   |\n\
|   |_________________________|     [ C3        ]    |_________________________|   |\n\
|   |          *              |     [ C4        ]    |                  *      |   |\n\
|   | y5  y4  y3  y2  y1  y0  |     =============    | r12 r11 r10 r9  r8  r7  |   |\n\
|   |.........................|                      |.........................|   |\n\
|                             |......|........|......|                             |\n\
|                             |  g12 |   g17  |  g0  |                             |\n\
|                             |  g11 |   g16  |  g1  |                             |\n\
|   =============             |______|________|______|           =============     |\n\
|   [           ]             |  g10 |   g15  |  g2  |           [           ]     |\n\
|   [  G1   G2  ]             |  g9  |   g14  |* g3  |           [  R1   R2  ]     |\n\
|   [           ]             |______|________|______|           [           ]     |\n\
|   [  G3   G4  ]             |* g8  | ~ g13  |  g4  |           [  R3   R4  ]     |\n\
|   [           ]             |  g7  |   g6   |  g5  |           [           ]     |\n\
|   =============             |......|........|......|           =============     |\n\
____________________________________________________________________________________\n\
\n";

void printBoard()
{
    char extraInfo[400];
    memset(extraInfo, '\0', 400);
    int combo = 0;
    char* c = BOARD_DESIGN;
    while ((*c)!='\0')
    {
        int num;
        if ((*c) == 'r' || (*c) == 'b' || (*c) == 'y' || (*c) == 'g')
        {
            char cc[16][2];
            short n = getCellContent(c, cc);
            if (n == 1)                     
            {                               
                putchar(cc[0][0]);          
                putchar(cc[0][1]);          
                putchar(' ');               
            }                               
            else if (n < 1)                 
            {                               
                printf("   ");
            }
            else
            {
                combo++;
                char text[4];
                text[0] = 'X'; text[1] = combo + '0'; text[2] = ' '; text[3] = '\0';
                printf(text);
                strcat(extraInfo, text);
                strcat(extraInfo, "has ");
                for (int i = 0; i < n; i++)
                {
                    strncat(extraInfo, cc[i], 2);
                    strcat(extraInfo, " ");
                }
                strcat(extraInfo, "\n");
            }
            c += 3;
        }
        else if ((*c) == 'R' || (*c) == 'B' || (*c) == 'Y' || (*c) == 'G')
        {
            if (isinHell(c))
            {
                putchar(*c);
                putchar(*(c + 1));
            }
            else
            {
                printf("  ");
            }
            c += 2;
        }
        else if ((*c) == 'C')
        {
            short index = atoi(c + 1);
            enum Color col = getRank(index - 1);
            char msg[6];
            memset(msg, ' ', 6);
            msg[0] = colorToChar(col);
            for (int i = 0; i < 4; i++)
            {
                if (isinHeaven(col, i))
                    msg[i + 1] = i + '1';
            }
            msg[5] = '\0';
            printf(msg);
            c += 5;
        }
        else
        {
            putchar(*c);
            c++;
        }
    }
    puts(extraInfo);
}