#include "board.h"
#include "chess_engine.h"
#include <game.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void play_game(void) {
  Game game;
  ColorPiece current_color;
  char input[10];
  Move move;
  bool player_is_white;
  bool game_over = false;
  bool player_turn;

  // Inicializar juego
  game.board = create_board(8, 8);
  init_standard_board(game.board);
  game.turn = WHITE_TURN;

  // Elegir color del jugador
  printf("¿Quieres jugar con blancas o negras? (b/n): ");
  if (fgets(input, sizeof(input), stdin) != NULL &&
      (input[0] == 'b' || input[0] == 'B')) {
    player_is_white = true;
  } else {
    player_is_white = false;
  }

  while (!game_over) {
    print_board(game.board);
    current_color = (game.turn == WHITE_TURN) ? WHITE : BLACK;
    player_turn = (player_is_white && current_color == WHITE) ||
                  (!player_is_white && current_color == BLACK);

    if (player_turn) {
      printf("Tu turno (%s). Ingresa tu movimiento (e.g., e2e4): ",
             current_color == WHITE ? "Blancas" : "Negras");
      if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Entrada inválida.\n");
        continue;
      }

      if (!parse_move_input(input, &move)) {
        printf("Formato inválido. Usa formato como e2e4.\n");
        continue;
      }

      if (!is_move_legal(game.board, move, current_color)) {
        printf("Movimiento ilegal.\n");
        continue;
      }

      apply_move(game.board, move);
    } else {
      printf("Turno de la máquina (%s)...\n",
             current_color == WHITE ? "Blancas" : "Negras");
      move = get_best_move(&game, 3); // Profundidad fija (puedes ajustar)
      apply_move(game.board, move);
      printf("La máquina juega: %c%d -> %c%d\n", move.from.col + 'a',
             8 - move.from.row, move.to.col + 'a', 8 - move.to.row);
    }

    // Verificar fin de juego
    if (is_checkmate(&game)) {
      print_board(game.board);
      printf("¡Jaque mate! Ganador: %s\n", player_turn ? "Jugador" : "Máquina");
      game_over = true;
    } else if (is_stalemate(&game)) {
      print_board(game.board);
      printf("¡Empate por ahogado!\n");
      game_over = true;
    } else {
      // Cambiar turno
      game.turn = (game.turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN;
    }
  }

  // Liberar memoria
  free_board(game.board);
}
