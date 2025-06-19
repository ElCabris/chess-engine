#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

MoveList get_all_legal_moves(const Board *board, ColorPiece color) {
  /* --- Declaraciones al inicio (C90 compliant) --- */
  MoveList all_moves, piece_moves, legal_moves;
  unsigned int row, col;
  Piece current_piece;

  /* --- Inicializaciones --- */
  all_moves.moves = NULL;
  all_moves.count = 0;
  legal_moves.moves = NULL;
  legal_moves.count = 0;

  /* 1. Validación de parámetros */
  if (!board) {
    return legal_moves;
  }

  /* 2. Recorrer todo el tablero */
  for (row = 0; row < board->rows; ++row) {
    for (col = 0; col < board->cols; ++col) {
      current_piece = board->elements[row][col];

      /* 3. Solo considerar piezas del color especificado */
      if (current_piece.color == color && current_piece.type != NO_TYPE) {
        Position from = {row, col};

        /* 4. Obtener movimientos válidos para la pieza */
        switch (current_piece.type) {
        case PAWN:
          piece_moves = get_valid_moves_pawn(board, from);
          break;
        case KNIGHT:
          piece_moves = get_valid_moves_knight(board, from);
          break;
        case BISHOP:
          piece_moves = get_valid_moves_bishop(board, from);
          break;
        case ROOK:
          piece_moves = get_valid_moves_rook(board, from);
          break;
        case QUEEN:
          piece_moves = get_valid_moves_queen(board, from);
          break;
        case KING:
          piece_moves = get_valid_moves_king(board, from);
          break;
        case NO_TYPE:
          piece_moves = (MoveList){NULL, 0};
          break;
        default:
          piece_moves = (MoveList){NULL, 0};
          break;
        }

        /* 5. Combinar movimientos */
        for (unsigned int i = 0; i < piece_moves.count; ++i) {
          Move *temp =
              realloc(all_moves.moves, sizeof(Move) * (all_moves.count + 1));
          if (!temp) {
            free_move_list(&all_moves);
            free_move_list(&piece_moves);
            return legal_moves;
          }
          all_moves.moves = temp;
          all_moves.moves[all_moves.count++] = piece_moves.moves[i];
        }

        free_move_list(&piece_moves);
      }
    }
  }

  /* 6. Filtrar solo movimientos legales (que no dejen al rey en jaque) */
  legal_moves = filter_legal_moves(board, all_moves, color);

  /* 7. Liberar memoria temporal */
  free_move_list(&all_moves);

  return legal_moves;
}
