#include <board.h>
#include <stdlib.h>

MoveList get_valid_moves_pawn(const Board *board, Position from) {
  MoveList list = {.moves = NULL, .count = 0};
  Piece pawn = board->elements[from.row][from.col];
  int direction = (pawn.color == WHITE) ? -1 : 1;
  int deltas[] = {-1, 1};

  // Declarar todas las variables al inicio (requerido en C90)
  unsigned int one_row, two_row;
  int diag_col;
  unsigned int diag_row;
  Position diag;

  // Verificar que el movimiento hacia adelante no resulte en un valor negativo
  if (direction == -1 && from.row == 0) {
    return list; // No hay movimientos posibles hacia arriba si ya está en la
                 // fila 0
  }

  one_row = (direction == -1) ? from.row - 1 : from.row + 1;
  two_row = (direction == -1) ? from.row - 2 : from.row + 2;

  if (is_on_board(board, (Position){one_row, from.col}) &&
      board->elements[one_row][from.col].type == NO_TYPE) {
    list.moves = realloc(list.moves, sizeof(Move) * (list.count + 1));
    list.moves[list.count++] = (Move){from, (Position){one_row, from.col}};

    if (!pawn.has_moved && is_on_board(board, (Position){two_row, from.col}) &&
        board->elements[two_row][from.col].type == NO_TYPE) {
      list.moves = realloc(list.moves, sizeof(Move) * (list.count + 1));
      list.moves[list.count++] = (Move){from, (Position){two_row, from.col}};
    }
  }

  for (int i = 0; i < 2; ++i) {
    diag_col = (int)from.col + deltas[i];
    if (diag_col < 0)
      continue; // Evitar valores negativos

    diag_row = (direction == -1) ? from.row - 1 : from.row + 1;
    diag = (Position){diag_row, (unsigned int)diag_col};

    if (is_on_board(board, diag)) {
      Piece target = board->elements[diag.row][diag.col];
      if (target.type != NO_TYPE && target.color != pawn.color) {
        list.moves = realloc(list.moves, sizeof(Move) * (list.count + 1));
        list.moves[list.count++] = (Move){from, diag};
      }
    }
  }

  return list;
}
