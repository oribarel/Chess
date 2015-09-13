#ifndef BEST_H
#define BEST_H

#include "Chess_Logic.h"

#define DOUBLED_PAWN_PENALTY		10
#define ISOLATED_PAWN_PENALTY		20
#define BACKWARDS_PAWN_PENALTY		8
#define PASSED_PAWN_BONUS			20
#define ROOK_SEMI_OPEN_FILE_BONUS	10
#define ROOK_OPEN_FILE_BONUS		15
#define ROOK_ON_SEVENTH_BONUS		20

#define PAWN_BEST_SCORE			0
#define KNIGHT_BEST_SCORE		1
#define BISHOP_BEST_SCORE		2
#define ROOK_BEST_SCORE			3
#define QUEEN_BEST_SCORE		4
#define KING_BEST_SCORE			5

#define LIGHT	0
#define DARK	1



int bestScore(board_t board, int player);
eTool get_eToolFromType(char type);
int Material(board_t board, int player);
int GetBestDepth(board_t board, int player);
int getDepth(board_t board, int player);
#endif
