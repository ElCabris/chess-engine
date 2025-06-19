#include <board.h>
#include <stdbool.h>

bool is_stalemate(const Game *game) {
  ColorPiece current_color;
  MoveList legal_moves;

  if (!game || !game->board || game->game_over) {
    return false;
  }

  // Determinar el color del jugador actual
  current_color = (game->turn == WHITE_TURN) ? WHITE : BLACK;

  // Si está en jaque, no es ahogado
  if (is_in_check(game->board, current_color)) {
    return false;
  }

  // Obtener todos los movimientos legales
  legal_moves = get_all_legal_moves(game->board, current_color);

  // Si no hay movimientos y no está en jaque → ahogado
  if (legal_moves.count == 0) {
    free_move_list(&legal_moves);
    return true;
  }

  free_move_list(&legal_moves);
  return false;
}
