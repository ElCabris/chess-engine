#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

MoveList get_valid_moves_knight(const Board *board, Position from) {
  MoveList list;
  Piece knight;
  int deltas[8][2];
  int i;
  int new_row, new_col;
  Position to;
  Piece target;
  Move *temp;

  list.moves = NULL;
  list.count = 0;

  if (!board || !is_on_board(board, from)) {
    return list;
  }

  knight = board->elements[from.row][from.col];
  if (knight.type != KNIGHT) {
    return list;
  }

  /* Todos los desplazamientos posibles del caballo */
  deltas[0][0] = -2;
  deltas[0][1] = -1;
  deltas[1][0] = -2;
  deltas[1][1] = 1;
  deltas[2][0] = -1;
  deltas[2][1] = -2;
  deltas[3][0] = -1;
  deltas[3][1] = 2;
  deltas[4][0] = 1;
  deltas[4][1] = -2;
  deltas[5][0] = 1;
  deltas[5][1] = 2;
  deltas[6][0] = 2;
  deltas[6][1] = -1;
  deltas[7][0] = 2;
  deltas[7][1] = 1;

  for (i = 0; i < 8; ++i) {
    new_row = (int)from.row + deltas[i][0];
    new_col = (int)from.col + deltas[i][1];

    /* Verificación de límites antes de crear Position */
    if (new_row < 0 || new_row >= (int)board->rows || new_col < 0 ||
        new_col >= (int)board->cols) {
      continue;
    }

    to.row = (unsigned int)new_row;
    to.col = (unsigned int)new_col;
    target = board->elements[to.row][to.col];

    if (target.type == NO_TYPE || target.color != knight.color) {
      temp = realloc(list.moves, sizeof(Move) * (list.count + 1));
      if (!temp) {
        free(list.moves);
        list.moves = NULL;
        list.count = 0;
        return list;
      }
      list.moves = temp;
      list.moves[list.count++] = (Move){from, to};
    }
  }

  return list;
}
