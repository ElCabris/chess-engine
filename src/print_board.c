#include <board.h>
#include <stdio.h>

void print_board(const Board *board) {
  unsigned int i, j;
  const char *piece_symbols[] = {"Kk", "Pp", "Nn", "Bb", "Rr", "Qq", "."};

  if (!board || !board->elements) {
    fprintf(stderr, "Tablero no válido\n");
    return;
  }

  printf("  ");
  for (j = 0; j < board->cols; ++j) {
    printf("%c ", 'a' + j);
  }
  printf("\n");

  for (i = 0; i < board->rows; ++i) {
    printf("%d ", board->rows - i);
    for (j = 0; j < board->cols; ++j) {
      Piece p = board->elements[i][j];

      if (p.type >= NO_TYPE || p.color > BLACK) {
        printf("? ");
      } else if (p.type == NO_TYPE) {
        printf(". ");
      } else {
        char symbol = piece_symbols[p.type][p.color == WHITE ? 0 : 1];
        printf("%c ", symbol);
      }
    }
    printf("\n");
  }
}
