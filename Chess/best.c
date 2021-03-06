#include "best.h"

/* The best scoring function in this file is based upon our learnings from the TSCP found in here: http://www.tckerrigan.com/Chess/TSCP/
And is modified to our needs. */


/* the values of the pieces */
int piece_value[6] = { 100, 300, 300, 500, 900, 0 };

/* Bonuses Matrices*/
int pawnBonus_col7[BOARD_SIZE] = { 0, 0, 1, 2, 3, 4, 5, 0 };
int pawnBonus_col6[BOARD_SIZE] = { 0, 0, 2, 4, 6, 8, 10, 0 };
int pawnBonus_col5[BOARD_SIZE] = { 0, 0, 3, 6, 9, 12, 15, 0 };
int pawnBonus_col4[BOARD_SIZE] = { 0, -40, 8, 8, 12, 16, 20, 0 };
int pawnBonus_col3[BOARD_SIZE] = { 0, -40, 8, 8, 12, 16, 20, 0 };
int pawnBonus_col2[BOARD_SIZE] = { 0, 0, 3, 6, 9, 12, 15, 0 };
int pawnBonus_col1[BOARD_SIZE] = { 0, 0, 2, 4, 6, 8, 10, 0 };
int pawnBonus_col0[BOARD_SIZE] = { 0, 0, 1, 2, 3, 4, 5, 0 };

int *pawnBonus[BOARD_SIZE] =
{
	pawnBonus_col0,
	pawnBonus_col1,
	pawnBonus_col2,
	pawnBonus_col3,
	pawnBonus_col4,
	pawnBonus_col5,
	pawnBonus_col6,
	pawnBonus_col7
};


int knightBonus_col7[BOARD_SIZE] = { -10, -10, -10, -10, -10, -10, -10, -10 };
int knightBonus_col6[BOARD_SIZE] = { -30, 0, 0, 0, 0, 0, 0, -10 };
int knightBonus_col5[BOARD_SIZE] = { -10, 0, 5, 5, 5, 5, 0, -10 };
int knightBonus_col4[BOARD_SIZE] = { -10, 0, 5, 10, 10, 5, 0, -10 };
int knightBonus_col3[BOARD_SIZE] = { -10, 0, 5, 10, 10, 5, 0, -10 };
int knightBonus_col2[BOARD_SIZE] = { -10, 0, 5, 5, 5, 5, 0, -10 };
int knightBonus_col1[BOARD_SIZE] = { -30, 0, 0, 0, 0, 0, 0, -10 };
int knightBonus_col0[BOARD_SIZE] = { -10, -10, -10, -10, -10, -10, -10, -10 };

int *knightBonus[BOARD_SIZE] =
{
	knightBonus_col0,
	knightBonus_col1,
	knightBonus_col2,
	knightBonus_col3,
	knightBonus_col4,
	knightBonus_col5,
	knightBonus_col6,
	knightBonus_col7
};


int bishopBonus_col7[BOARD_SIZE] = { -10, -10, -10, -10, -10, -10, -10, -10 };
int bishopBonus_col6[BOARD_SIZE] = { -10, 0, 0, 0, 0, 0, 0, -10 };
int bishopBonus_col5[BOARD_SIZE] = { -20, 0, 5, 5, 5, 5, 0, -10 };
int bishopBonus_col4[BOARD_SIZE] = { -10, 0, 5, 10, 10, 5, 0, -10 };
int bishopBonus_col3[BOARD_SIZE] = { -10, 0, 5, 10, 10, 5, 0, -10 };
int bishopBonus_col2[BOARD_SIZE] = { -20, 0, 5, 5, 5, 5, 0, -10 };
int bishopBonus_col1[BOARD_SIZE] = { -10, 0, 0, 0, 0, 0, 0, -10 };
int bishopBonus_col0[BOARD_SIZE] = { -10, -10, -10, -10, -10, -10, -10, -10 };

int *bishopBonus[BOARD_SIZE] =
{
	bishopBonus_col0,
	bishopBonus_col1,
	bishopBonus_col2,
	bishopBonus_col3,
	bishopBonus_col4,
	bishopBonus_col5,
	bishopBonus_col6,
	bishopBonus_col7
};



int kingBonus_col7[BOARD_SIZE] = { 20, -20, -40, -40, -40, -40, -40, -40 };
int kingBonus_col6[BOARD_SIZE] = { 40, -20, -40, -40, -40, -40, -40, -40 };
int kingBonus_col5[BOARD_SIZE] = { 5, -20, -40, -40, -40, -40, -40, -40 };
int kingBonus_col4[BOARD_SIZE] = { 0, -20, -40, -40, -40, -40, -40, -40 };
int kingBonus_col3[BOARD_SIZE] = { 5, -20, -40, -40, -40, -40, -40, -40 };
int kingBonus_col2[BOARD_SIZE] = { 40, -20, -40, -40, -40, -40, -40, -40 };
int kingBonus_col1[BOARD_SIZE] = { 20, -20, -40, -40, -40, -40, -40, -40 };
int kingBonus_col0[BOARD_SIZE] = { 0, -20, -40, -40, -40, -40, -40, -40 };

int *kingBonus[BOARD_SIZE] =
{
	kingBonus_col0,
	kingBonus_col1,
	kingBonus_col2,
	kingBonus_col3,
	kingBonus_col4,
	kingBonus_col5,
	kingBonus_col6,
	kingBonus_col7
};



int kingEndGameBonus_col7[BOARD_SIZE] = { 0, 10, 20, 30, 30, 20, 10, 0 };
int kingEndGameBonus_col6[BOARD_SIZE] = { 10, 20, 30, 40, 40, 30, 20, 10 };
int kingEndGameBonus_col5[BOARD_SIZE] = { 20, 30, 40, 50, 50, 40, 30, 20 };
int kingEndGameBonus_col4[BOARD_SIZE] = { 30, 40, 50, 60, 60, 50, 40, 30 };
int kingEndGameBonus_col3[BOARD_SIZE] = { 30, 40, 50, 60, 60, 50, 40, 30 };
int kingEndGameBonus_col2[BOARD_SIZE] = { 20, 30, 40, 50, 50, 40, 30, 20 };
int kingEndGameBonus_col1[BOARD_SIZE] = { 10, 20, 30, 40, 40, 30, 20, 10 };
int kingEndGameBonus_col0[BOARD_SIZE] = { 0, 10, 20, 30, 30, 20, 10, 0 };

int *kingEndGameBonus[BOARD_SIZE] =
{
	kingEndGameBonus_col0,
	kingEndGameBonus_col1,
	kingEndGameBonus_col2,
	kingEndGameBonus_col3,
	kingEndGameBonus_col4,
	kingEndGameBonus_col5,
	kingEndGameBonus_col6,
	kingEndGameBonus_col7
};


/* pawnRow[x][y] is the row of the least advanced pawn of color x on column
y - 1. There are extra columns on the left and right to make easy logics later.
If there's no pawn on a rank, we pretend the pawn is
impossibly far advanced (8 for LIGHT and -1 for DARK). */

int pawnRow[2][BOARD_SIZE + 2];

int piecesScoreValue[2];  /* the value of a side's pieces */
int pawnsScoreValue[2];  /* the value of a side's pawns */

/*Util*/
eTool get_eToolFromType(char type)
{
	if (type == WHITE_P || type == BLACK_P)
		return Pawn;
	else if (type == WHITE_N || type == BLACK_N)
		return Knight;
	else if (type == WHITE_B || type == BLACK_B)
		return Bishop;
	else if (type == WHITE_R || type == BLACK_R)
		return Rook;
	else if (type == WHITE_Q || type == BLACK_Q)
		return Queen;
	else if (type == WHITE_K || type == BLACK_K)
		return King;
	else
		return Empty;
}



int bestScore(board_t board, int player)
{
	int column;
	int score[2];  /* each side's score */
	int ally, enemy, playerBlocked = 1, opponentBlocked = 1;
	Coord crd;

	/* Check for win or tie situations: */
	/* Since getMoves doesn't provide any moves that leave the king at CHECK, if there aren't any moves, it is MATE */
	
	
	int canMove = 0;
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE; k++)
	{
		crd.i_coord = (int)mod(k, BOARD_SIZE);
		crd.j_coord = k / BOARD_SIZE;
		ally = IsAlly(GetContentOfCoord(board, crd), player);
		enemy = IsEnemy(GetContentOfCoord(board, crd), player);
		if (GetContentOfCoord(board, crd) != EMPTY)
		{
			if (opponentBlocked || playerBlocked)
			{
				canMove = canMoveThisTool(board, crd);
				if (ally > 0 && canMove == 1)
					playerBlocked = 0;
				if (enemy > 0 && canMove == 1)
					opponentBlocked = 0;
			}
		}
	}
	if (playerBlocked == 1)
	{
		if (KingUnderThreat(board, player))
			return MATE_WIN_LOSE;//player lost
		else
			return TIE_SCORE; //tie
	}
	if (opponentBlocked == 1)
	{
		if (KingUnderThreat(board, -player))
			return MATE_WIN_SCORE;//player won
		else
			return TIE_SCORE; //tie
	}
	
	
	
	/* set up pawnRow, piecesScoreValue, and pawnsScoreValue. */
	for (int i = 0; i < 10; ++i)
	{
		// Pawns are set to impossibly far locations. 
		pawnRow[LIGHT][i] = BOARD_SIZE;
		pawnRow[DARK][i] = -1;
	}

	piecesScoreValue[LIGHT] = 0;
	piecesScoreValue[DARK] = 0;
	pawnsScoreValue[LIGHT] = 0;
	pawnsScoreValue[DARK] = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			if (GetContentOfCoord(board, crd) == EMPTY)
				continue;
			if (GetContentOfCoord(board, crd) == WHITE_P || GetContentOfCoord(board, crd) == BLACK_P)
			{
				pawnsScoreValue[getColorInLightOrDark(board, crd)] += piece_value[0];
				// reminder: int piece_value[6] = {100, 300, 300, 500, 900, 0};

				column = i + 1;  /* add 1 because of the extra column in the array */
				if (getColorInLightOrDark(board, crd) == LIGHT)
				{
					if (pawnRow[LIGHT][column] > j)
						pawnRow[LIGHT][column] = j;
				}
				else
				{
					if (pawnRow[DARK][column] < j)
						pawnRow[DARK][column] = j;
				}
			}
			else
				piecesScoreValue[getColorInLightOrDark(board, crd)] += piece_value[(int)get_eToolFromType(GetContentOfCoord(board, crd))];
		}
	}

	/* evaluate each piece */
	score[LIGHT] = piecesScoreValue[LIGHT] + pawnsScoreValue[LIGHT];
	score[DARK] = piecesScoreValue[DARK] + pawnsScoreValue[DARK];
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			if (GetContentOfCoord(board, crd) == EMPTY)
				continue;
			if (getColorInLightOrDark(board, crd) == LIGHT)
			{
				char tool = GetContentOfCoord(board, crd);
				if (tool == WHITE_P)
					score[LIGHT] += scoreWhitePawns(crd);
				else if (tool == WHITE_N)
					score[LIGHT] += knightBonus[i][j];
				else if (tool == WHITE_B)
					score[LIGHT] += bishopBonus[i][j];
				else if (tool == WHITE_R)
				{
					if (pawnRow[LIGHT][i + 1] == BOARD_SIZE)
					{
						if (pawnRow[DARK][i + 1] == -1)
							score[LIGHT] += ROOK_OPEN_FILE_BONUS;
						else
							score[LIGHT] += ROOK_SEMI_OPEN_FILE_BONUS;
					}
					if (j == 6)
						score[LIGHT] += ROOK_ON_SEVENTH_BONUS;
				}
				else if (tool == WHITE_K)
				{
					if (piecesScoreValue[DARK] <= 1200)
						score[LIGHT] += kingEndGameBonus[i][j];
					else
						score[LIGHT] += scoreWhiteKing(crd);
				}
			}
			else // DARK
			{
				char tool = GetContentOfCoord(board, crd);
				if (tool == BLACK_P)
					score[DARK] += scoreBlackPawns(crd);
				else if (tool == BLACK_N)
					score[DARK] += knightBonus[i][BOARD_SIZE - 1 - j];
				else if (tool == BLACK_B)
					score[DARK] += bishopBonus[i][BOARD_SIZE - 1 - j];
				else if (tool == BLACK_R)
				{

					if (pawnRow[DARK][i + 1] == -1)
					{
						if (pawnRow[LIGHT][i + 1] == BOARD_SIZE)
							score[DARK] += ROOK_OPEN_FILE_BONUS;
						else
							score[DARK] += ROOK_SEMI_OPEN_FILE_BONUS;
					}
					if (j == 1)
						score[DARK] += ROOK_ON_SEVENTH_BONUS;
				}
				else if (tool == BLACK_K)
				{
					if (piecesScoreValue[LIGHT] <= 1200)
						score[DARK] += kingEndGameBonus[i][BOARD_SIZE - 1 - j];
					else
						score[DARK] += scoreBlackKing(crd);
				}
			}
		}
	}

	/* the score[] array is set, now return the score relative
	to the side to move */
	if (player == WHITE_PLAYER)
		return score[LIGHT] - score[DARK];
	return score[DARK] - score[LIGHT];
}

/*UNUSED*/
int Material(board_t board, int player)
{
	Coord crd;
	int column;
	int score[2];  /* each side's score */
	int ally, enemy, playerBlocked = 1, opponentBlocked = 1;

	/* Check for win or tie situations: */
	/* Since getMoves doesn't provide any moves that leave the king at CHECK, if there aren't any moves, it is MATE */
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE; k++)
	{
		crd.i_coord = (int)mod(k, BOARD_SIZE);
		crd.j_coord = k / BOARD_SIZE;
		ally = IsAlly(GetContentOfCoord(board, crd), player);
		enemy = IsEnemy(GetContentOfCoord(board, crd), player);
		if (GetContentOfCoord(board, crd) != EMPTY)
		{
			if (ally > 0 && canMoveThisTool(board, crd) == 1)
				playerBlocked = 0;
			if (enemy > 0 && canMoveThisTool(board, crd) == 1)
				opponentBlocked = 0;
		}
	}
	if (playerBlocked && !opponentBlocked)
		return -1000000;
	else if (playerBlocked && opponentBlocked)
		return -999999;
	else if (opponentBlocked)
		return 1000000;

	/* this is the first pass: set up pawnRow, piecesScoreValue, and pawnsScoreValue. */
	for (int i = 0; i < 10; ++i)
	{
		// Pawns are set to impossible locations. 
		pawnRow[LIGHT][i] = BOARD_SIZE;
		pawnRow[DARK][i] = -1;
	}

	piecesScoreValue[LIGHT] = 0;
	piecesScoreValue[DARK] = 0;
	pawnsScoreValue[LIGHT] = 0;
	pawnsScoreValue[DARK] = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			if (GetContentOfCoord(board, crd) == EMPTY)
				continue;
			if (GetContentOfCoord(board, crd) == WHITE_P || GetContentOfCoord(board, crd) == BLACK_P)
			{
				pawnsScoreValue[getColorInLightOrDark(board, crd)] += piece_value[0]; // reminder: int piece_value[6] = {100, 300, 300, 500, 900, 0};

				column = i + 1;  /* add 1 because of the extra column in the array */
				if (getColorInLightOrDark(board, crd) == LIGHT)
				{
					if (pawnRow[LIGHT][column] > j)
						pawnRow[LIGHT][column] = j;
				}
				else
				{
					if (pawnRow[DARK][column] < j)
						pawnRow[DARK][column] = j;
				}
			}
			else
				piecesScoreValue[getColorInLightOrDark(board, crd)] += piece_value[(int)get_eToolFromType(GetContentOfCoord(board, crd))];
		}
	}

	/* this is the second pass: evaluate each piece */
	score[LIGHT] = piecesScoreValue[LIGHT] + pawnsScoreValue[LIGHT];
	score[DARK] = piecesScoreValue[DARK] + pawnsScoreValue[DARK];
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			if (GetContentOfCoord(board, crd) == EMPTY)
				continue;
			if (getColorInLightOrDark(board, crd) == LIGHT)
			{
				char tool = GetContentOfCoord(board, crd);
				if (tool == WHITE_P)
					score[LIGHT] += scoreWhitePawns(crd);
				else if (tool == WHITE_N)
					score[LIGHT] += knightBonus[i][j];
				else if (tool == WHITE_B)
					score[LIGHT] += bishopBonus[i][j];
				else if (tool == WHITE_R)
				{
					if (pawnRow[LIGHT][i + 1] == BOARD_SIZE)
					{
						if (pawnRow[DARK][i + 1] == -1)
							score[LIGHT] += ROOK_OPEN_FILE_BONUS;
						else
							score[LIGHT] += ROOK_SEMI_OPEN_FILE_BONUS;
					}
					if (j == 6)
						score[LIGHT] += ROOK_ON_SEVENTH_BONUS;
				}
				else if (tool == WHITE_K)
				{
					if (piecesScoreValue[DARK] <= 1200)
						score[LIGHT] += kingEndGameBonus[i][j];
					else
						score[LIGHT] += scoreWhiteKing(crd);
				}
			}
			else // DARK
			{
				char tool = GetContentOfCoord(board, crd);
				if (tool == BLACK_P)
					score[DARK] += scoreBlackPawns(crd);
				else if (tool == BLACK_N)
					score[DARK] += knightBonus[i][BOARD_SIZE - 1 - j];
				else if (tool == BLACK_B)
					score[DARK] += bishopBonus[i][BOARD_SIZE - 1 - j];
				else if (tool == BLACK_R)
				{

					if (pawnRow[DARK][i + 1] == -1)
					{
						if (pawnRow[LIGHT][i + 1] == BOARD_SIZE)
							score[DARK] += ROOK_OPEN_FILE_BONUS;
						else
							score[DARK] += ROOK_SEMI_OPEN_FILE_BONUS;
					}
					if (j == 1)
						score[DARK] += ROOK_ON_SEVENTH_BONUS;
				}
				else if (tool == BLACK_K)
				{
					if (piecesScoreValue[LIGHT] <= 1200)
						score[DARK] += kingEndGameBonus[i][BOARD_SIZE - 1 - j];
					else
						score[DARK] += scoreBlackKing(crd);
				}
			}
		}
	}


	/* the score[] array is set, now return the score relative
	to the side to move */
	if (player == WHITE_PLAYER)
		return score[LIGHT];
	return score[DARK];
}

int scoreWhitePawns(Coord crd)
{
	int res;
	int column;

	res = 0;
	column = crd.j_coord + 1;

	res += pawnBonus[crd.i_coord][crd.j_coord];

	/* a penalty for a doubled pawn: https://en.wikipedia.org/wiki/Doubled_pawns */
	if (pawnRow[LIGHT][column] < crd.j_coord)
		res -= DOUBLED_PAWN_PENALTY;

	/* penalty for an isolated pawn: https://en.wikipedia.org/wiki/Isolated_pawn */
	if ((pawnRow[LIGHT][column - 1] == BOARD_SIZE) && (pawnRow[LIGHT][column + 1] == BOARD_SIZE))
		res -= ISOLATED_PAWN_PENALTY;

	/* penalty for a backwards Pawn: https://en.wikipedia.org/wiki/Backward_pawn */
	else if ((pawnRow[LIGHT][column - 1] > crd.j_coord) && (pawnRow[LIGHT][column + 1] > crd.j_coord))
		res -= BACKWARDS_PAWN_PENALTY;

	/* bonus for a passed pawn https://en.wikipedia.org/wiki/Passed_pawn */
	if ((pawnRow[DARK][column - 1] <= crd.j_coord) && (pawnRow[DARK][column] <= crd.j_coord) &&
		(pawnRow[DARK][column + 1] <= crd.j_coord))
		res += crd.j_coord * PASSED_PAWN_BONUS;

	return res;
}

int scoreBlackPawns(Coord crd)
{
	int res;
	int column;

	res = 0;
	column = crd.j_coord + 1;

	res += pawnBonus[crd.i_coord][BOARD_SIZE - 1 - crd.j_coord];

	/* a penalty for a doubled pawn: https://en.wikipedia.org/wiki/Doubled_pawns */
	if (pawnRow[DARK][column] > crd.j_coord)
		res -= DOUBLED_PAWN_PENALTY;

	/* penalty for an isolated pawn: https://en.wikipedia.org/wiki/Isolated_pawn */
	if ((pawnRow[DARK][column - 1] == -1) && (pawnRow[DARK][column + 1] == -1))
		res -= ISOLATED_PAWN_PENALTY;

	/* penalty for a backwards Pawn: https://en.wikipedia.org/wiki/Backward_pawn */
	else if ((pawnRow[DARK][column - 1] < crd.j_coord) && (pawnRow[DARK][column + 1] < crd.j_coord))
		res -= BACKWARDS_PAWN_PENALTY;


	/* bonus for a passed pawn https://en.wikipedia.org/wiki/Passed_pawn */
	if ((pawnRow[LIGHT][column - 1] >= crd.j_coord) && (pawnRow[LIGHT][column] >= crd.j_coord) &&
		(pawnRow[LIGHT][column + 1] >= crd.j_coord))
		res += (BOARD_SIZE - 1 - crd.j_coord) * PASSED_PAWN_BONUS;

	return res;
}

int scoreWhiteKing(Coord crd)
{
	int res;

	res = kingBonus[crd.i_coord][crd.j_coord];

	/* if the king is castled, use a special function to evaluate the
	pawns on the appropriate side */
	if (crd.i_coord < 3)
	{
		res += scoreWhiteKingPawns(1);
		res += scoreWhiteKingPawns(2);
		res += scoreWhiteKingPawns(3) / 2;  /* problems with pawns on the c & f files
							   are not as severe */
	}
	else if (crd.i_coord > 4)
	{
		res += scoreWhiteKingPawns(8);
		res += scoreWhiteKingPawns(7);
		res += scoreWhiteKingPawns(6) / 2;
	}

	/* otherwise, just assess a penalty if there are open files near
	the king */
	else
	{
		for (int k = crd.i_coord; k <= crd.i_coord + 2; k++)
		{
			if ((pawnRow[LIGHT][k] == BOARD_SIZE) && (pawnRow[DARK][k] == -1))
				res -= 10;
		}
	}

	/* scale the king score according to the opponent's material */

	/*res *= piecesScoreValue[DARK];
	res /= 3100;*/

	res *= 3100;
	res /= piecesScoreValue[DARK]; 

	return res;
}

/* evaluates the White King Pawn on a column */
int scoreWhiteKingPawns(int column)
{
	int res = 0;

	/* pawn moved one square */
	if (pawnRow[LIGHT][column] == 2)
		res -= 10;
	/* pawn moved more than one square */
	else if (pawnRow[LIGHT][column] > 2 && pawnRow[LIGHT][column] < BOARD_SIZE)
		res -= 20;

	//no pawn on the column means 8 for white or -1 for black
	/* no pawn on this column */
	else  if (pawnRow[LIGHT][column] == BOARD_SIZE)
		res -= 25;

	/* no enemy pawn */
	if (pawnRow[DARK][column] == -1)
		res -= 15;

	/* enemy pawn on the 3rd row */
	else if (pawnRow[DARK][column] == 2)
		res -= 10;

	/* enemy pawn on the 4th row */
	else if (pawnRow[DARK][column] == 3)
		res -= 5;

	return res;
}

int scoreBlackKing(Coord crd)
{
	int res;

	res = kingBonus[crd.i_coord][BOARD_SIZE - 1 - crd.j_coord];
	if (crd.i_coord < 3)
	{
		res += scoreBlackKingPawns(1);
		res += scoreBlackKingPawns(2);
		res += scoreBlackKingPawns(3) / 2;
	}
	else if (crd.i_coord > 4)
	{
		res += scoreBlackKingPawns(8);
		res += scoreBlackKingPawns(7);
		res += scoreBlackKingPawns(6) / 2;
	}
	else
	{
		for (int i = crd.i_coord; i <= crd.i_coord + 2; i++)
		{
			if ((pawnRow[LIGHT][i] == BOARD_SIZE) && (pawnRow[DARK][i] == -1))
				res -= 10;
		}
	}

	/*res *= piecesScoreValue[LIGHT];
	res /= 3100;*/

	res *= 3100;
	res /= piecesScoreValue[LIGHT];


	return res;
}

int scoreBlackKingPawns(int column)
{
	int res = 0;

	if (pawnRow[DARK][column] == 5)
		res -= 10;
	else if (pawnRow[DARK][column] < 5 && pawnRow[DARK][column] > -1)
		res -= 20;
	else if (pawnRow[DARK][column] == -1)
		res -= 25;

	if (pawnRow[LIGHT][column] == BOARD_SIZE)
		res -= 15;
	else if (pawnRow[LIGHT][column] == 6)
		res -= 10;
	else if (pawnRow[LIGHT][column] == 5)
		res -= 5;

	return res;
}

int getColorInLightOrDark(board_t board, Coord crd)
{
	if (GetContentOfCoord(board, crd) == EMPTY)
		return -1;
	else if (getColor(board, crd) == WHITE_PLAYER)
		return LIGHT;
	else
		return DARK;
}

/*This function assesses the depth to run the minimax with in order to not exceed boards limitation*/
int GetBestDepth(board_t board, int player)
{
	int numberOfWhitePawns = 0, numberOfBlackPawns = 0;
	int numberOfWhiteRooks = 0, numberOfBlackRooks = 0;
	int numberOfWhiteBishops = 0, numberOfBlackBishops = 0;
	int numberOfWhiteKnights = 0, numberOfBlackKnights = 0;
	int numberOfWhiteQueens = 0, numberOfBlackQueens = 0;
	int pawnMaxMoves = 3, knightMaxMoves = 8, bishopMaxMoves = 13, rookMaxMoves = 14, queenMaxMoves = 27, kingMaxMoves = 8;
	int movesCnt = 1;
	int numberOfWhiteMoves;
	int numberOfBlackMoves;
	char c;
	int depth = 1;
	int numberOfPlayerMoves;
	int numberOfOpponentMoves;

	//counter number of tools of each type
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			c = board[i][j];
			switch (c)
			{
			case BLACK_P:
				numberOfBlackPawns++;
				break;
			case BLACK_B:
				numberOfBlackBishops++;
				break;
			case BLACK_N:
				numberOfBlackKnights++;
				break;
			case BLACK_R:
				numberOfBlackRooks++;
				break;
			case BLACK_Q:
				numberOfBlackQueens++;
				break;
			case BLACK_K:
				break;
			case WHITE_P:
				numberOfWhitePawns++;
				break;
			case WHITE_B:
				numberOfWhiteBishops++;
				break;
			case WHITE_N:
				numberOfWhiteKnights++;
				break;
			case WHITE_R:
				numberOfWhiteRooks++;
				break;
			case WHITE_Q:
				numberOfWhiteQueens++;
				break;
			case WHITE_K:
				break;
			default:
				break;
			}
		}


	numberOfWhiteMoves = numberOfWhitePawns * pawnMaxMoves
		+ numberOfWhiteRooks * rookMaxMoves + numberOfWhiteBishops * bishopMaxMoves +
		numberOfWhiteKnights * knightMaxMoves + numberOfWhiteQueens*queenMaxMoves + kingMaxMoves;
	numberOfBlackMoves = numberOfBlackPawns * pawnMaxMoves
		+ numberOfBlackRooks * rookMaxMoves + numberOfBlackBishops * bishopMaxMoves +
		numberOfBlackKnights * knightMaxMoves + numberOfBlackQueens*queenMaxMoves + kingMaxMoves;
	numberOfPlayerMoves = player == WHITE_PLAYER ? numberOfWhiteMoves : numberOfBlackMoves;
	numberOfOpponentMoves = player == WHITE_PLAYER ? numberOfBlackMoves : numberOfWhiteMoves;

	while (movesCnt < BEST_BOARDS_NUM)
	{
		if (depth % 2 == 1)
			movesCnt *= numberOfPlayerMoves;
		else
			movesCnt *= numberOfOpponentMoves;
		depth++;
	}
	if (DEBUG)
		printf("best depth is: %d\n", depth - 1);
	return --depth; //TODO: how much we want?

}


int getDepth(board_t board, int player)
{
	if (properties[2] == 0)
		return GetBestDepth(board, player);
	else
		return properties[2];
}