#include <board.h>

bool is_on_board(const Board *board, Position pos) {
  if (!board)
    return false;

  return pos.row >= 0 && pos.row < board->rows && pos.col >= 0 &&
         pos.col < board->cols;
}
