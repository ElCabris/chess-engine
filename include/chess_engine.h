#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H
#include <board.h>

int evaluate_board(const Board *board);
Move get_best_move(const Game *game, int depth);
#endif
