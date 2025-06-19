#include <board.h>

int main(void) {
  Board *board = create_board(8, 8);
  init_standard_board(board);
  print_board(board);

  free_board(board);
  return 0;
}
