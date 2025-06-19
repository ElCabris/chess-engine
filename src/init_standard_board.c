#include <board.h>
void init_standard_board(Board *board) {
  for (unsigned int j = 0; j < board->cols; j++) {
    board->elements[1][j] = (Piece){PAWN, BLACK, false};
    board->elements[6][j] = (Piece){PAWN, WHITE, false};
  }

  board->elements[0][0] = (Piece){ROOK, BLACK, false};
  board->elements[0][1] = (Piece){KNIGHT, BLACK, false};
  board->elements[0][2] = (Piece){BISHOP, BLACK, false};
  board->elements[0][3] = (Piece){QUEEN, BLACK, false};
  board->elements[0][4] = (Piece){KING, BLACK, false};
  board->elements[0][5] = (Piece){BISHOP, BLACK, false};
  board->elements[0][6] = (Piece){KNIGHT, BLACK, false};
  board->elements[0][7] = (Piece){ROOK, BLACK, false};

  board->elements[7][0] = (Piece){ROOK, WHITE, false};
  board->elements[7][1] = (Piece){KNIGHT, WHITE, false};
  board->elements[7][2] = (Piece){BISHOP, WHITE, false};
  board->elements[7][3] = (Piece){QUEEN, WHITE, false};
  board->elements[7][4] = (Piece){KING, WHITE, false};
  board->elements[7][5] = (Piece){BISHOP, WHITE, false};
  board->elements[7][6] = (Piece){KNIGHT, WHITE, false};
  board->elements[7][7] = (Piece){ROOK, WHITE, false};
}
