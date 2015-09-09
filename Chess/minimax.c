
#include "minimax.h"

int mini_num = 0;

/* the minimax function as instructed.
pay attention: this function only returns integers. the argument bestMove
is an address to a pointer that exists outside of minimax_score, and when obtaining the information about
which move is truely optimal (due to depth restrictions) the content of the address is set to the address of that said move. */
int minimax_score(board_t board, int player, int depth, int minOrMax, cMove **bestMove, int a, int b, int boardsCounter)
{
	cMove *movesList, *tmp;
	int bestValue, val;
	if (depth == 0)//base case
	{
		if (minOrMax == 1)// 1 for maximizing player
			return score(board, player);
		else
			return score(board, -player);
	}
	if (minOrMax == 1)// 1 for maximizing player
	{
		bestValue = MIN_VALUE;
		movesList = getMoves(board, player);
		tmp = movesList;


		while (movesList != NULL)
		{
			int bestUpdated = 0;
			//compute score and return board to its original state
			val = makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax, a, b, boardsCounter);
			if (depth == properties[2] && val >= bestValue) //if depth == minimax_depth
			{
				if (val == bestValue)
					*bestMove = AddMove(bestMove, movesList->toolType, movesList->src, movesList->dst, movesList->eaten, movesList->promote);
				else
				{
					if (*bestMove != NULL)
					{
						freeMovesList(*bestMove);
						*bestMove = NULL;
					}
					*bestMove = AddMove(bestMove, movesList->toolType, movesList->src, movesList->dst, movesList->eaten, movesList->promote);
					//(*bestMove) = tmp;
					//(*bestMove)->next = NULL;
				}
				bestUpdated = 1;
			}
			bestValue = imax(bestValue, val);
			a = imax(a, bestValue);
			movesList = movesList->next;
			if (bestUpdated == 0)
			{
				free(tmp);
			}
			tmp = movesList;
			if (b <= a)
				break; // b cut-off
		}
		return bestValue;
	}
	else
	{
		bestValue = MAX_VALUE;
		movesList = getMoves(board, player);
		tmp = movesList;

		while (movesList != NULL)
		{
			int bestUpdated = 0;
			//compute score and return board to its original state
			val = makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax, a, b, boardsCounter);
			if (depth == properties[2] && val <= bestValue) //if depth == minimax_depth
			{
				if (val == bestValue)
					*bestMove = AddMove(bestMove, movesList->toolType, movesList->src, movesList->dst, movesList->eaten, movesList->promote);
				else
				{
					if (*bestMove != NULL)
					{
						freeMovesList(*bestMove);
						*bestMove = NULL;
					}
					*bestMove = AddMove(bestMove, movesList->toolType, movesList->src, movesList->dst, movesList->eaten, movesList->promote);
					//(*bestMove) = tmp;
					//(*bestMove)->next = NULL;
				}
				bestUpdated = 1;
			}
			bestValue = imin(bestValue, val);
			b = imin(b, bestValue);
			movesList = movesList->next;
			if (bestUpdated == 0)
			{
				free(tmp);
			}
			tmp = movesList;

			if (b <= a)
				break; // a cut-off

		}
		return bestValue;
	}

	return 0;
}

















/*Called from within the minimax_score func. make reversable alterations on the board: see description inside.*/
int makeMove_ComputeScore_Undo(board_t board, cMove *move, int player, int depth, int minOrMax, int a, int b, int boardsCounter)
/*Saves all the eaten tool types in the eatenTools array, all while making the move on the given board.
Then, run minimax algorithm in recursion to produce a score. At last, undo the move, using the array, and return the score computed.*/
{
	int scr = 0;
	//making the move
	makeMove(board, move);

	//make a recursive call to minimax_score and update the nuber of boards that have already been evaluated	
	scr = minimax_score(board, -player, depth - 1, 1 - minOrMax, NULL, a, b, ++boardsCounter);

	//undo the move - reconstruct the board as before
	setSlotInBoard(board, move->src, move->toolType);
	if (move->eaten == 0)
		setSlotInBoard(board, move->dst, EMPTY);
	else
		setSlotInBoard(board, move->dst, move->eaten);

	return scr;

}