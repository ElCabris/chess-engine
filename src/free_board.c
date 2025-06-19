#include <board.h>
#include <stdlib.h>

void free_board(Board *board) {
  if (board == NULL) {
    return;
  }

  if (board->elements != NULL) {
    for (unsigned int i = 0; i < board->rows; i++) {
      if (board->elements[i] != NULL) {
        free(board->elements[i]);
      }
    }
    free(board->elements);
  }

  free(board);
}
