#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

MoveList filter_legal_moves(const Board *board, MoveList raw_moves,
                            ColorPiece color) {
  /* --- Declaraciones al inicio (C90 compliant) --- */
  MoveList legal_moves;
  unsigned int i;
  Board *simulated_board;
  Piece original_piece;
  Piece empty_piece = {NO_TYPE, NO_COLOR, false};
  bool is_legal;

  /* --- Inicializaciones --- */
  legal_moves.moves = NULL;
  legal_moves.count = 0;
  simulated_board = NULL;

  /* 1. Validación de parámetros */
  if (!board || !raw_moves.moves) {
    return legal_moves;
  }

  /* 2. Filtrar movimientos legales */
  for (i = 0; i < raw_moves.count; ++i) {
    /* Crear tablero simulado para cada movimiento */
    simulated_board = malloc(sizeof(Board));
    if (!simulated_board) {
      free_move_list(&legal_moves);
      return (MoveList){NULL, 0};
    }

    /* Copiar estructura básica */
    simulated_board->rows = board->rows;
    simulated_board->cols = board->cols;

    /* Reservar memoria para el array 2D */
    simulated_board->elements = malloc(board->rows * sizeof(Piece *));
    if (!simulated_board->elements) {
      free(simulated_board);
      free_move_list(&legal_moves);
      return (MoveList){NULL, 0};
    }

    /* Copiar cada fila */
    for (unsigned int j = 0; j < board->rows; ++j) {
      simulated_board->elements[j] = malloc(board->cols * sizeof(Piece));
      if (!simulated_board->elements[j]) {
        /* Liberar memoria ya asignada */
        for (unsigned int k = 0; k < j; ++k) {
          free(simulated_board->elements[k]);
        }
        free(simulated_board->elements);
        free(simulated_board);
        free_move_list(&legal_moves);
        return (MoveList){NULL, 0};
      }
      for (unsigned int k = 0; k < board->cols; ++k) {
        simulated_board->elements[j][k] = board->elements[j][k];
      }
    }

    /* Aplicar movimiento en el tablero simulado */
    original_piece = simulated_board->elements[raw_moves.moves[i].from.row]
                                              [raw_moves.moves[i].from.col];
    simulated_board
        ->elements[raw_moves.moves[i].to.row][raw_moves.moves[i].to.col] =
        original_piece;
    simulated_board
        ->elements[raw_moves.moves[i].from.row][raw_moves.moves[i].from.col] =
        empty_piece;

    /* Verificar si el movimiento deja al rey en jaque */
    is_legal = !is_in_check(simulated_board, color);

    /* Liberar memoria del tablero simulado */
    for (unsigned int j = 0; j < simulated_board->rows; ++j) {
      free(simulated_board->elements[j]);
    }
    free(simulated_board->elements);
    free(simulated_board);

    /* Si es legal, añadir a la lista */
    if (is_legal) {
      Move *temp =
          realloc(legal_moves.moves, sizeof(Move) * (legal_moves.count + 1));
      if (!temp) {
        free_move_list(&legal_moves);
        return (MoveList){NULL, 0};
      }
      legal_moves.moves = temp;
      legal_moves.moves[legal_moves.count++] = raw_moves.moves[i];
    }
  }

  return legal_moves;
}
