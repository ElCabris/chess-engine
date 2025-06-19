#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

MoveList get_valid_moves_queen(const Board *board, Position from) {
  MoveList list;
  Piece queen;
  int directions[8][2];
  int d, dr, dc;
  Position current;
  Piece target;
  Move *temp;
  unsigned int new_row, new_col;

  list.moves = NULL;
  list.count = 0;

  if (!board || !is_on_board(board, from)) {
    return list;
  }

  queen = board->elements[from.row][from.col];
  if (queen.type != QUEEN) {
    return list;
  }

  /* Direcciones: ortogonales + diagonales */
  directions[0][0] = -1;
  directions[0][1] = 0; // ↑
  directions[1][0] = 1;
  directions[1][1] = 0; // ↓
  directions[2][0] = 0;
  directions[2][1] = -1; // ←
  directions[3][0] = 0;
  directions[3][1] = 1; // →
  directions[4][0] = -1;
  directions[4][1] = -1; // ↖
  directions[5][0] = -1;
  directions[5][1] = 1; // ↗
  directions[6][0] = 1;
  directions[6][1] = -1; // ↙
  directions[7][0] = 1;
  directions[7][1] = 1; // ↘

  for (d = 0; d < 8; ++d) {
    dr = directions[d][0];
    dc = directions[d][1];
    current = from;

    while (1) {
      if ((dr == -1 && current.row == 0) || (dc == -1 && current.col == 0)) {
        break;
      }

      new_row = (dr == -1)  ? current.row - 1
                : (dr == 1) ? current.row + 1
                            : current.row;
      new_col = (dc == -1)  ? current.col - 1
                : (dc == 1) ? current.col + 1
                            : current.col;

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
        if (target.color != queen.color) {
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
