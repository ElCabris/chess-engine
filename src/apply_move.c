#include <board.h>
#include <stdbool.h>

void apply_move(Board *board, Move move) {
  /* --- Declaraciones al inicio (C90 compliant) --- */
  Piece moving_piece;
  Piece empty_piece = {NO_TYPE, NO_COLOR, false};

  /* 1. Validación de parámetros */
  if (!board || !is_on_board(board, move.from) ||
      !is_on_board(board, move.to)) {
    return;
  }

  /* 2. Obtener la pieza que se mueve */
  moving_piece = board->elements[move.from.row][move.from.col];

  /* 3. Verificar que hay una pieza para mover */
  if (moving_piece.type == NO_TYPE) {
    return;
  }

  /* 4. Aplicar el movimiento */
  board->elements[move.to.row][move.to.col] = moving_piece;
  board->elements[move.from.row][move.from.col] = empty_piece;

  /* 5. Actualizar estado de la pieza (ej. peón que se mueve por primera vez) */
  if (!moving_piece.has_moved) {
    board->elements[move.to.row][move.to.col].has_moved = true;
  }

  /* Nota: Aquí deberías añadir lógica para movimientos especiales como:
     - Enroque
     - Captura al paso
     - Promoción de peón */
}
