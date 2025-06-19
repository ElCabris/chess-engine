#include <board.h>
#include <stdbool.h>
#include <stdlib.h>

Board *copy_board(const Board *board) {
  /* --- Todas las declaraciones al inicio --- */
  Board *new_board;
  unsigned int i, j;
  bool allocation_failed;
  Piece **elements;
  unsigned int rows, cols;

  /* --- Inicializaciones --- */
  new_board = NULL;
  allocation_failed = false;
  elements = NULL;
  rows = 0;
  cols = 0;

  /* 1. Verificar entrada */
  if (!board) {
    return NULL;
  }

  /* 2. Reservar memoria para la estructura principal */
  new_board = malloc(sizeof(Board));
  if (!new_board) {
    return NULL;
  }

  /* 3. Copiar dimensiones básicas */
  rows = board->rows;
  cols = board->cols;
  new_board->rows = rows;
  new_board->cols = cols;

  /* 4. Reservar memoria para el array de filas */
  elements = malloc(rows * sizeof(Piece *));
  if (!elements) {
    free(new_board);
    return NULL;
  }
  new_board->elements = elements;

  /* 5. Reservar memoria para cada fila */
  for (i = 0; i < rows; ++i) {
    elements[i] = malloc(cols * sizeof(Piece));
    if (!elements[i]) {
      allocation_failed = true;
      break;
    }
  }

  /* 6. Manejar fallo de asignación */
  if (allocation_failed) {
    for (j = 0; j < i; ++j) {
      free(elements[j]);
    }
    free(elements);
    free(new_board);
    return NULL;
  }

  /* 7. Copiar cada pieza */
  for (i = 0; i < rows; ++i) {
    for (j = 0; j < cols; ++j) {
      elements[i][j] = board->elements[i][j];
    }
  }

  return new_board;
}
