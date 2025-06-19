#include <board.h>
#include <stdlib.h>

void free_move_list(MoveList *list) {
  if (list && list->moves) {
    free(list->moves);
    list->moves = NULL;
    list->count = 0;
  }
}
