
#include "minimax.h"

int mini_num = 0;

/* the minimax function as instructed.
pay attention: this function only returns integers. the argument bestMove
is an address to a pointer that exists outside of minimax_score, and when obtaining the information about
which move is truely optimal (due to depth restrictions) the content of the address is set to the address of that said move. */
int minimax_score(board_t board, int player, int depth, int minOrMax, cMove **bestMove, int a, int b, int best)
{
	cMove *movesList, *tmp;
	int bestValue, val;
	if (depth == 0)//base case
	{
		if (minOrMax == 1)// 1 for maximizing player
		{
			if (best == 0)
				return score(board, player);
			else
				return bestScore(board, player);
		}
		else
		{
			if (best == 0)
				return score(board, -player);
			else
				return bestScore(board, -player);
		}
	}


	int scr = score(board, player); //if terminal node
	if (scr == MATE_WIN_LOSE || scr == MATE_WIN_SCORE || scr == TIE_SCORE)
	{
		if (minOrMax == 1)// 1 for maximizing player
		{
			if (scr == TIE_SCORE)
				return -scr;
			return scr;
		}
		else
		{
			if (scr == TIE_SCORE)
				return scr;
			return -scr;
		}
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
			val = makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax, a, b, best);
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
			//if (bestUpdated == 0)
			//{
				free(tmp);
			//}
			tmp = movesList;
			if (b < a)
			{
				freeMovesList(movesList);
				break; // b cut-off
			}
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
			val = makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax, a, b, best);


			/* I think this is very unnecessary*/
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
			/* End unnecessary */

			bestValue = imin(bestValue, val);
			b = imin(b, bestValue);
			movesList = movesList->next;
			//if (bestUpdated == 0)
			//{
				free(tmp);
			//}
			tmp = movesList;

			if (b < a)
			{
				freeMovesList(movesList);
				break; // a cut-off
			}

		}
		return bestValue;
	}

	return 0;
}


int minimax_score2(board_t board, int player, int depth, int maximizingPlayer, cMove **bestMove, int a, int b, int best)
{
	cMove *movesList, *head;
	int bestValue, val, immediateWin = 0;

	//end case
	if (depth == 0)
	{
		if (best == 0)
			return score(board, player);
		else
			return bestScore(board, player);
	}

	//if terminal node
	int scr = score(board, player);
	if (scr == MATE_WIN_LOSE || scr == MATE_WIN_SCORE || scr == TIE_SCORE)
	{
		return scr;
	}




	movesList = getMoves(board, player);
	movesList = mergeSort_movesList(movesList, board, player);
	head = movesList;


	if (maximizingPlayer)
	{
		bestValue = MIN_VALUE;


		while (movesList != NULL)
		{

			/* Only on uppermost level, Check if can win immediately */
			if (depth == properties[2] && canWinImmediately(board, movesList, player))
			{
				bestValue = MATE_WIN_SCORE;
				if (immediateWin == 0) //first time we see an immediate win move
				{
					if (*bestMove != NULL)
					{
						freeMovesList(*bestMove);
						*bestMove = NULL;
					}
				}
				immediateWin = 1;
				*bestMove = AddMove(bestMove, movesList->toolType, movesList->src, movesList->dst, movesList->eaten, movesList->promote);
			}
			else if (!immediateWin)
			{

				//compute score and return board to its original state
				val = makeMove_ComputeScore_Undo(board, movesList, player, depth, maximizingPlayer, a, b, best);


				if (depth == properties[2] && val >= bestValue && !immediateWin) //if depth == minimax_depth
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
					}
				}

				bestValue = imax(bestValue, val);
				a = imax(a, bestValue);
				if (b < a)
					break; // b cut-off
			}

			movesList = movesList->next;

		}
		freeMovesList(head);
		return bestValue;
	}
	else // minimizing player
	{
		bestValue = MAX_VALUE;

		while (movesList != NULL)
		{
			//compute score and return board to its original state
			val = makeMove_ComputeScore_Undo(board, movesList, player, depth, maximizingPlayer, a, b, best);

			bestValue = imin(bestValue, val);
			b = imin(b, bestValue);
			movesList = movesList->next;

			if (b < a)
				break; // a cut-off

		}
		freeMovesList(head);
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




int canWinImmediately(board_t board, cMove *movesList, int player)
{
	makeMove(board, movesList);
	int scr = score(board, player);

	setSlotInBoard(board, movesList->src, movesList->toolType);
	if (movesList->eaten == 0)
		setSlotInBoard(board, movesList->dst, EMPTY);
	else
		setSlotInBoard(board, movesList->dst, movesList->eaten);

	if (scr == MATE_WIN_SCORE)
		return 1;
	return 0;
}


/*Called from within the minimax_score func. make reversable alterations on the board: see description inside.*/
int makeMove_ComputeScore_Undo2(board_t board, cMove *move, int player, int depth, int maximizingPlayer, int a, int b, int best)
/*Makes the move on the given board. Then, runs minimax algorithm in recursion to produce a score. At last, undos the move and returns the score computed.*/
{
	int scr = 0;
	//making the move
	makeMove(board, move);

	//make a recursive call to minimax_score and update the nuber of boards that have already been evaluated	
	scr = minimax_score(board, player, depth - 1, !maximizingPlayer, NULL, a, b, best);

	//undo the move - reconstruct the board as before
	setSlotInBoard(board, move->src, move->toolType);
	if (move->eaten == 0)
		setSlotInBoard(board, move->dst, EMPTY);
	else
		setSlotInBoard(board, move->dst, move->eaten);

	return scr;

}

cMove *mergeSort_movesList(cMove *list, board_t board, int player)
{
	cMove *p, *q, *e, *tail, *oldhead;
	int insize, nmerges, psize, qsize, i;

	if (list == NULL)
		return NULL;

	insize = 1;
	while (1)
	{
		p = list;
		oldhead = list;		       /* only used for circular linkage */
		list = NULL;
		tail = NULL;

		nmerges = 0;  /* count number of merges we do in this pass */

		while (p)
		{
			nmerges++;  /* there exists a merge to be done */
			/* step `insize' places along from p */
			q = p;
			psize = 0;
			for (i = 0; i < insize; i++)
			{
				psize++;
				q = q->next;
				if (!q) break;
			}

			/* if q hasn't fallen off end, we have two lists to merge */
			qsize = insize;

			/* now we have two lists; merge them */
			while (psize > 0 || (qsize > 0 && q))
			{

				/* decide whether next element of merge comes from p or q */
				if (psize == 0)
				{
					/* p is empty; e must come from q. */
					e = q; q = q->next; qsize--;
				}
				else if (qsize == 0 || !q)
				{
					/* q is empty; e must come from p. */
					e = p; p = p->next; psize--;
				}
				else if (cmp(p, q, board, player) >= 0)
				{
					/* First element of p is higher (or same);
					 * e must come from p. */
					e = p; p = p->next; psize--;
				}
				else
				{
					/* First element of q is higher; e must come from q. */
					e = q; q = q->next; qsize--;
				}

				/* add the next element to the merged list */
				if (tail)
				{
					tail->next = e;
				}
				else
				{
					list = e;
				}
				tail = e;
			}

			/* now p has stepped `insize' places along, and q has too */
			p = q;
		}

		tail->next = NULL;

		/* If we have done only one merge, we're finished. */
		if (nmerges <= 1)   /* allow for nmerges==0, the empty list case */
			return list;

		/* Otherwise repeat, merging lists twice the size */
		insize *= 2;
	}
}

int cmp(cMove *a, cMove *b, board_t board, int player)
{
	int scr;
	if (a->scoreWhenMade == -1000002)
	{
		makeMove(board, a);
		scr = score(board, player);

		setSlotInBoard(board, a->src, a->toolType);
		if (a->eaten == 0)
			setSlotInBoard(board, a->dst, EMPTY);
		else
			setSlotInBoard(board, a->dst, a->eaten);

		a->scoreWhenMade = scr;
	}
	if (b->scoreWhenMade == -1000002)
	{
		makeMove(board, b);
		scr = score(board, player);

		setSlotInBoard(board, b->src, b->toolType);
		if (b->eaten == 0)
			setSlotInBoard(board, b->dst, EMPTY);
		else
			setSlotInBoard(board, b->dst, b->eaten);

		b->scoreWhenMade = scr;
	}
	return a->scoreWhenMade - b->scoreWhenMade;
}
