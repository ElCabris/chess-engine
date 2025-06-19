#include <board.h>
#include <stdbool.h>
void make_move(Game *game, Move move) {
  ColorPiece current_color;

  if (!game || !game->board || game->game_over)
    return;

  current_color = (game->turn == WHITE_TURN) ? WHITE : BLACK;

  // 1. Validar que el movimiento sea legal
  if (!is_move_legal(game->board, move, current_color)) {
    return;
  }

  // 2. Aplicar el movimiento
  apply_move(game->board, move);
  game->last_move = move;

  // 3. Cambiar el turno
  game->turn = (game->turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN;

  // 4. Verificar si el juego terminó (jaque mate o ahogado)
  if (is_checkmate(game)) {
    game->game_over = true;
    game->winner = current_color; // el jugador que hizo el jaque mate gana
  } else if (is_stalemate(game)) {
    game->game_over = true;
    game->winner = NO_COLOR; // empate
  }

  // ⚠️ Aquí puedes agregar más condiciones de empate si deseas (material
  // insuficiente, repetición, 50 movimientos, etc.)
}
