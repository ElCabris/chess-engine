#include <board.h>
#include <stdbool.h>

bool is_checkmate(const Game *game) {
  ColorPiece current_color;
  MoveList legal_moves;

  if (!game || !game->board || game->game_over) {
    return false;
  }

  // Determinar el color del jugador actual
  current_color = (game->turn == WHITE_TURN) ? WHITE : BLACK;

  // Si el rey no está en jaque, no es jaque mate
  if (!is_in_check(game->board, current_color)) {
    return false;
  }

  // Obtener todos los movimientos legales disponibles
  legal_moves = get_all_legal_moves(game->board, current_color);

  // Si no hay movimientos legales y está en jaque, es jaque mate
  if (legal_moves.count == 0) {
    free_move_list(&legal_moves);
    return true;
  }

  // Si hay movimientos legales, no es jaque mate
  free_move_list(&legal_moves);
  return false;
}
