#include <board.h>

bool is_on_board(const Board *board, Position pos) {
  if (!board)
    return false;

  return pos.row < board->rows && pos.col < board->cols;
}
