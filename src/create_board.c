#include <board.h>
#include <stdlib.h>

Board *create_board(unsigned int rows, unsigned int cols) {
  Board *board = (Board *)malloc(sizeof(Board));
  board->rows = rows;
  board->cols = cols;

  board->elements = (Piece **)malloc(rows * sizeof(Piece *));

  for (unsigned int i = 0; i < rows; ++i) {
    board->elements[i] = (Piece *)malloc(cols * sizeof(Piece));
    for (unsigned int j = 0; j < cols; ++j) {
      board->elements[i][j].type = NO_TYPE;
      board->elements[i][j].color = NO_COLOR;
    }
  }
  return board;
}
