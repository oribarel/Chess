#ifndef MINIMAX_H
#define MINIMAX_H

#include "best.h"

int minimax_score(board_t board, int player, int depth, int minOrMax, cMove **bestMove, int a, int b, int best);
int makeMove_ComputeScore_Undo(board_t board, cMove *move, int player, int depth, int minOrMax, int a, int b, int boardsCounter);
#endif
