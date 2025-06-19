#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_move_legal(const Board *board, Move move, ColorPiece color) {
  /* --- Declaraciones al inicio (C90 compliant) --- */
  Piece moving_piece;
  MoveList valid_moves;
  unsigned int i, j;
  Board *simulated_board;
  bool is_legal;
  Piece empty_piece;

  /* --- Inicializaciones --- */
  valid_moves.moves = NULL;
  valid_moves.count = 0;
  simulated_board = NULL;
  is_legal = false;
  empty_piece.type = NO_TYPE;
  empty_piece.color = NO_COLOR;
  empty_piece.has_moved = false;

  /* 1. Validaciones iniciales */
  if (!board || !is_on_board(board, move.from) ||
      !is_on_board(board, move.to)) {
    return false;
  }

  moving_piece = board->elements[move.from.row][move.from.col];

  /* 2. Verificar propiedad de la pieza */
  if (moving_piece.color != color || moving_piece.type == NO_TYPE) {
    return false;
  }

  /* 3. Obtener movimientos válidos */
  switch (moving_piece.type) {
  case PAWN:
    valid_moves = get_valid_moves_pawn(board, move.from);
    break;
  case KNIGHT:
    valid_moves = get_valid_moves_knight(board, move.from);
    break;
  case BISHOP:
    valid_moves = get_valid_moves_bishop(board, move.from);
    break;
  case ROOK:
    valid_moves = get_valid_moves_rook(board, move.from);
    break;
  case QUEEN:
    valid_moves = get_valid_moves_queen(board, move.from);
    break;
  case KING:
    valid_moves = get_valid_moves_king(board, move.from);
    break;
  case NO_TYPE: /* Explícitamente manejado aunque ya filtrado */
  default:      /* Para cualquier otro valor no esperado */
    return false;
  }

  /* 4. Verificar coincidencia con movimientos válidos */
  for (i = 0; i < valid_moves.count; ++i) {
    if (valid_moves.moves[i].to.row == move.to.row &&
        valid_moves.moves[i].to.col == move.to.col) {
      is_legal = true;
      break;
    }
  }
  free_move_list(&valid_moves);

  if (!is_legal) {
    return false;
  }

  /* 5. Simular el movimiento */
  simulated_board = malloc(sizeof(Board));
  if (!simulated_board) {
    return false;
  }

  simulated_board->rows = board->rows;
  simulated_board->cols = board->cols;

  simulated_board->elements = malloc(board->rows * sizeof(Piece *));
  if (!simulated_board->elements) {
    free(simulated_board);
    return false;
  }

  for (i = 0; i < board->rows; ++i) {
    simulated_board->elements[i] = malloc(board->cols * sizeof(Piece));
    if (!simulated_board->elements[i]) {
      for (j = 0; j < i; ++j) {
        free(simulated_board->elements[j]);
      }
      free(simulated_board->elements);
      free(simulated_board);
      return false;
    }
    for (j = 0; j < board->cols; ++j) {
      simulated_board->elements[i][j] = board->elements[i][j];
    }
  }

  /* Aplicar movimiento simulado */
  simulated_board->elements[move.to.row][move.to.col] = moving_piece;
  simulated_board->elements[move.from.row][move.from.col] = empty_piece;

  /* 6. Verificar jaque */
  is_legal = !is_in_check(simulated_board, color);

  free_board(simulated_board);

  return is_legal;
}
