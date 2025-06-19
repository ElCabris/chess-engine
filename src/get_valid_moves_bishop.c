#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

MoveList get_valid_moves_bishop(const Board *board, Position from) {
  /* --- Declaraciones al inicio (C90 compatible) --- */
  MoveList list;
  Piece bishop;
  int directions[4][2];
  int d, dr, dc;
  Position current;
  Piece target;
  Move *temp;
  unsigned int new_row, new_col;

  /* --- Inicializaciones --- */
  list.moves = NULL;
  list.count = 0;

  /* Validación de entrada */
  if (!board || !is_on_board(board, from)) {
    return list;
  }

  bishop = board->elements[from.row][from.col];
  if (bishop.type != BISHOP) {
    return list;
  }

  /* Direcciones diagonales: ↖ ↗ ↙ ↘ */
  directions[0][0] = -1;
  directions[0][1] = -1; // ↖
  directions[1][0] = -1;
  directions[1][1] = 1; // ↗
  directions[2][0] = 1;
  directions[2][1] = -1; // ↙
  directions[3][0] = 1;
  directions[3][1] = 1; // ↘

  /* --- Movimiento en cada dirección --- */
  for (d = 0; d < 4; ++d) {
    dr = directions[d][0];
    dc = directions[d][1];
    current = from;

    while (1) {
      /* Evitar underflow */
      if ((dr == -1 && current.row == 0) || (dc == -1 && current.col == 0)) {
        break;
      }

      new_row = (dr == -1) ? current.row - 1 : current.row + 1;
      new_col = (dc == -1) ? current.col - 1 : current.col + 1;

      if (new_row >= board->rows || new_col >= board->cols) {
        break;
      }

      current.row = new_row;
      current.col = new_col;
      target = board->elements[current.row][current.col];

      if (target.type == NO_TYPE) {
        temp = realloc(list.moves, sizeof(Move) * (list.count + 1));
        if (!temp) {
          free(list.moves);
          list.moves = NULL;
          list.count = 0;
          return list;
        }
        list.moves = temp;
        list.moves[list.count++] = (Move){from, current};
      } else {
        if (target.color != bishop.color) {
          temp = realloc(list.moves, sizeof(Move) * (list.count + 1));
          if (!temp) {
            free(list.moves);
            list.moves = NULL;
            list.count = 0;
            return list;
          }
          list.moves = temp;
          list.moves[list.count++] = (Move){from, current};
        }
        break;
      }
    }
  }

  return list;
}
