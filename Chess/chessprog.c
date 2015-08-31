/*
The structs are:
- Coord: a coordinate on the board. see header file.
- cMove(functions as a linked list node): which contains some data about the movement,
and also the type of the eaten tool if exists.

A brief explaination about implementation:
the board is two-dim char array, allocated statically only once in the main.
no board duplications were made. in order to achieve better performance the getMoves functions (of all kinds),
and the minimax function (including the	MakeMove_ComputeScore_Undo) make reversable alterations on the given board,
making board-duplications unnecessary.

public array properties contains 5 cells which are relevant throughtout the program. see below.

Functions that are labeled:
- DEBUG: are for debugging purposes only
- UNUSED: are unused although not deleted.
*/

#include "chessprog.h"

int main(int argc, char *argv[]) 
{
	/************************
	* Board Initialization  *
	*************************/
	char* board[BOARD_SIZE];
	char row0[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row1[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row2[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row3[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row4[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row5[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row6[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row7[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	board[0] = row0;
	board[1] = row1;
	board[2] = row2;
	board[3] = row3;
	board[4] = row4;
	board[5] = row5;
	board[6] = row6;
	board[7] = row7;

	if (argc == 1)
	{
		Console_Main(board);
	}
	else if (argc == 2 && strncmp(argv[1], "console", 7) == 0)
	{
		Console_Main(board);
	}
	else if (argc == 2 && strncmp(argv[1], "gui", 3) == 0)
	{
		GUI_Main(board);
	}
	return 0;
}
