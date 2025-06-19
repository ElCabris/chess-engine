#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_in_check(const Board *board, ColorPiece color) {
  /* --- Declaraciones al inicio (requerido en C90) --- */
  Position king_pos;
  bool king_found;
  ColorPiece enemy_color;
  unsigned int row, col, i;
  Piece current;
  Position from;
  MoveList moves;

  /* --- Inicializaciones --- */
  king_pos.row = 0;
  king_pos.col = 0;
  king_found = false;

  if (!board)
    return false;

  /* 1. Encontrar la posición del rey */
  for (row = 0; row < board->rows; ++row) {
    for (col = 0; col < board->cols; ++col) {
      current = board->elements[row][col];
      if (current.type == KING && current.color == color) {
        king_pos.row = row;
        king_pos.col = col;
        king_found = true;
        break;
      }
    }
    if (king_found)
      break;
  }

  if (!king_found)
    return false;

  /* 2. Determinar color enemigo */
  enemy_color = (color == WHITE) ? BLACK : WHITE;

  /* 3. Verificar ataques enemigos */
  for (row = 0; row < board->rows; ++row) {
    for (col = 0; col < board->cols; ++col) {
      current = board->elements[row][col];
      if (current.color == enemy_color) {
        from.row = row;
        from.col = col;
        moves.moves = NULL;
        moves.count = 0;

        /* Obtener movimientos válidos */
        switch (current.type) {
        case PAWN:
          moves = get_valid_moves_pawn(board, from);
          break;
        case KNIGHT:
          moves = get_valid_moves_knight(board, from);
          break;
        case BISHOP:
          moves = get_valid_moves_bishop(board, from);
          break;
        case ROOK:
          moves = get_valid_moves_rook(board, from);
          break;
        case QUEEN:
          moves = get_valid_moves_queen(board, from);
          break;
        case KING:
          moves = get_valid_moves_king(board, from);
          break;
        case NO_TYPE: /* Manejar caso NO_TYPE */
          break;
        default:
          break;
        }

        /* Verificar si algún movimiento ataca al rey */
        for (i = 0; i < moves.count; ++i) {
          if (moves.moves[i].to.row == king_pos.row &&
              moves.moves[i].to.col == king_pos.col) {
            free_move_list(&moves);
            return true;
          }
        }

        free_move_list(&moves);
      }
    }
  }

  return false;
}
