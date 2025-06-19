#include "chess_engine.h" // Asumo que board.h está incluido en chess_engine.h
#include <math.h>         // Para abs()
#include <stdbool.h>      // Para bool
#include <stdlib.h>       // Para abs()

// Valores de las piezas
#define PAWN_VALUE 100
#define KNIGHT_VALUE 320
#define BISHOP_VALUE 330
#define ROOK_VALUE 500
#define QUEEN_VALUE 900
#define KING_VALUE 20000

// Parámetros de evaluación
#define CENTER_CONTROL_PAWN 10
#define CENTER_CONTROL_PIECE 3
#define KING_SAFETY_PENALTY -10
#define DOUBLED_PAWN_PENALTY -15
#define ISOLATED_PAWN_PENALTY -20
#define PASSED_PAWN_BONUS 25
#define BISHOP_PAIR_BONUS 30
#define UNDEVELOPED_PENALTY -10
#define ROOK_BLOCKED_PENALTY -15
#define ROOK_OPEN_FILE_BONUS 15

int evaluate_board(const Board *board) {
  // --- C90 FIX: Declarar TODAS las variables al inicio de la función ---
  int score = 0;
  unsigned int r, c, k; // Contadores de bucles

  int white_material = 0, black_material = 0;
  int white_bishop_count = 0, black_bishop_count = 0;

  // Estas variables ahora se usarán para almacenar si el rey está en jaque,
  // y se inicializan a false para evitar errores de lectura.
  bool white_king_in_check_flag =
      false; // Renombrada para evitar conflicto con tu variable original
  bool black_king_in_check_flag = false; // Renombrada

  // Arrays para peones doblados, aislados, etc. (Tamaño fijo 8 para C90)
  int white_pawns_in_col_count[8] = {0};
  int black_pawns_in_col_count[8] = {0};
  bool has_white_pawn_in_col[8] = {false};
  bool has_black_pawn_in_col[8] = {false};

  // Variables temporales usadas dentro de los bucles
  Piece current;
  int value;
  int bonus;
  bool blocked;
  bool open_file;
  bool white_isolated;
  bool black_isolated;
  bool is_passed;
  bool endgame;

  if (!board || !board->elements) {
    return 0; // Tablero nulo o inválido
  }

  // PASADA 1: Material, centro, desarrollo, conteo de peones, búsqueda de jaque
  for (r = 0; r < board->rows; r++) {
    for (c = 0; c < board->cols; c++) {
      current = board->elements[r][c];
      if (current.type == NO_TYPE) {
        continue; // Casilla vacía, saltar
      }

      // Asignar valor a la pieza y actualizar material
      value = 0; // C90 FIX: Declarada al inicio, pero reiniciamos aquí
      switch (current.type) {
      case PAWN:
        value = PAWN_VALUE;
        break;
      case KNIGHT:
        value = KNIGHT_VALUE;
        break;
      case BISHOP:
        value = BISHOP_VALUE;
        break;
      case ROOK:
        value = ROOK_VALUE;
        break;
      case QUEEN:
        value = QUEEN_VALUE;
        break;
      case KING:
        value = KING_VALUE;
        break;
      case NO_TYPE: /* C90 FIX: Manejado para el switch */
        break;
      default:
        break; // Valor por defecto
      }

      if (current.color == WHITE) {
        white_material += value;
        score += value;
      } else {
        black_material += value;
        score -= value;
      }

      // Conteo de alfiles para el par de alfiles
      if (current.type == BISHOP) {
        if (current.color == WHITE)
          white_bishop_count++;
        else
          black_bishop_count++;
      }

      // Control del centro (peones y otras piezas)
      if ((r == 3 || r == 4) && (c == 3 || c == 4)) {
        bonus =
            (current.type == PAWN) ? CENTER_CONTROL_PAWN : CENTER_CONTROL_PIECE;
        score += (current.color == WHITE) ? bonus : -bonus;
      }

      // Conteo de peones por columna para futuras evaluaciones (doblados,
      // aislados)
      if (current.type == PAWN &&
          c < 8) { // Asegurarse de no exceder los límites del array
        if (current.color == WHITE) {
          white_pawns_in_col_count[c]++;
          has_white_pawn_in_col[c] = true;
        } else {
          black_pawns_in_col_count[c]++;
          has_black_pawn_in_col[c] = true;
        }
      }

      // Verificar si el rey está en jaque (una sola vez por rey)
      // NOTA: Esta lógica dentro del bucle pieza por pieza podría ser
      // ineficiente si se llama is_in_check para cada pieza de rey en cada
      // iteración. Es mejor buscar los reyes primero y luego llamar is_in_check
      // una sola vez para cada rey. Sin embargo, para mantener tu estructura de
      // una sola pasada principal, la adapto.
      if (current.type == KING) {
        if (current.color == WHITE)
          white_king_in_check_flag = is_in_check(board, WHITE);
        else
          black_king_in_check_flag = is_in_check(board, BLACK);
      }

      // Penalización por piezas no desarrolladas (caballos y alfiles en su fila
      // inicial)
      if ((current.type == KNIGHT || current.type == BISHOP) &&
          ((current.color == WHITE && r == 7) ||
           (current.color == BLACK && r == 0))) {
        score += (current.color == WHITE) ? UNDEVELOPED_PENALTY
                                          : -UNDEVELOPED_PENALTY;
      }

      // Penalización/bonificación de torres
      if (current.type == ROOK &&
          c < 8) { // Asegurarse de no exceder los límites del array
        // Penalización por torre bloqueada por su propio peón en la fila
        // inicial
        blocked = false; // C90 FIX: Declarada al inicio
        if (current.color == WHITE && r == 7) {
          if (board->elements[6][c].type == PAWN &&
              board->elements[6][c].color == WHITE) {
            blocked = true;
          }
        } else if (current.color == BLACK && r == 0) {
          if (board->elements[1][c].type == PAWN &&
              board->elements[1][c].color == BLACK) {
            blocked = true;
          }
        }
        if (blocked) {
          score += (current.color == WHITE) ? ROOK_BLOCKED_PENALTY
                                            : -ROOK_BLOCKED_PENALTY;
        }

        // Bonificación por torre en columna abierta (sin peones de ningún
        // color)
        open_file = !has_white_pawn_in_col[c] &&
                    !has_black_pawn_in_col[c]; // C90 FIX: Declarada al inicio
        if (open_file) {
          score += (current.color == WHITE) ? ROOK_OPEN_FILE_BONUS
                                            : -ROOK_OPEN_FILE_BONUS;
        }
      }
    }
  }

  // Bonificación por el par de alfiles (evaluada después de contar todos los
  // alfiles)
  if (white_bishop_count >= 2)
    score += BISHOP_PAIR_BONUS;
  if (black_bishop_count >= 2)
    score -= BISHOP_PAIR_BONUS;

  // PASADA 2: Peones doblados y aislados (basado en el conteo de la Pasada 1)
  for (c = 0; c < 8; c++) { // Iteramos sobre las 8 columnas fijas
    if (white_pawns_in_col_count[c] > 1)
      score += DOUBLED_PAWN_PENALTY;
    if (black_pawns_in_col_count[c] > 1)
      score -= DOUBLED_PAWN_PENALTY;

    // Peones aislados: Verificar si hay un peón en la columna 'c'
    // y si NO hay peones propios en las columnas adyacentes.
    white_isolated = has_white_pawn_in_col[c] && // C90 FIX: Declarada al inicio
                     !(c > 0 && has_white_pawn_in_col[c - 1]) &&
                     !(c < 7 && has_white_pawn_in_col[c + 1]);
    black_isolated = has_black_pawn_in_col[c] && // C90 FIX: Declarada al inicio
                     !(c > 0 && has_black_pawn_in_col[c - 1]) &&
                     !(c < 7 && has_black_pawn_in_col[c + 1]);

    if (white_isolated)
      score += ISOLATED_PAWN_PENALTY;
    if (black_isolated)
      score -= ISOLATED_PAWN_PENALTY;
  }

  // PASADA 3: Peones pasados
  for (r = 0; r < board->rows; r++) {
    for (c = 0; c < board->cols; c++) {
      Piece p = board->elements[r][c];
      if (p.type == PAWN) {
        is_passed = true; // C90 FIX: Declarada al inicio
        if (p.color == WHITE) {
          // Un peón blanco es pasado si no hay peones negros en su columna o
          // adyacentes delante de él
          for (k = r + 1; k < board->rows && is_passed; ++k) {
            if ((board->elements[k][c].type == PAWN &&
                 board->elements[k][c].color == BLACK) ||
                (c > 0 && board->elements[k][c - 1].type == PAWN &&
                 board->elements[k][c - 1].color == BLACK) ||
                (c < board->cols - 1 &&
                 board->elements[k][c + 1].type == PAWN &&
                 board->elements[k][c + 1].color == BLACK)) {
              is_passed = false;
            }
          }
          if (is_passed)
            score += PASSED_PAWN_BONUS;
        } else { // Color: BLACK
          // Un peón negro es pasado si no hay peones blancos en su columna o
          // adyacentes delante de él
          for (k = 0; k < r && is_passed; ++k) {
            if ((board->elements[k][c].type == PAWN &&
                 board->elements[k][c].color == WHITE) ||
                (c > 0 && board->elements[k][c - 1].type == PAWN &&
                 board->elements[k][c - 1].color == WHITE) ||
                (c < board->cols - 1 &&
                 board->elements[k][c + 1].type == PAWN &&
                 board->elements[k][c + 1].color == WHITE)) {
              is_passed = false;
            }
          }
          if (is_passed)
            score -= PASSED_PAWN_BONUS;
        }
      }
    }
  }

  // --- Evaluación de la Fase del Juego y Seguridad/Centralización del Rey ---
  // Determinamos si es un final de juego basándonos en el material restante.
  // Los valores de 2400 para material son un umbral arbitrario; puedes
  // ajustarlos.
  endgame = (white_material <= 2400 ||
             black_material <= 2400); // C90 FIX: Declarada al inicio

  if (endgame) {
    // En finales, el rey se vuelve una pieza activa y debe centralizarse.
    // Penalizamos la distancia del rey al centro del tablero (3,3 o e4/d5).
    for (r = 0; r < board->rows; r++) {
      for (c = 0; c < board->cols; c++) {
        Piece king = board->elements[r][c];
        if (king.type == KING) {
          // Distancia de Manhattan al centro (ej. (3,3) para tablero
          // 0-indexado)
          int dist = abs((int)r - 3) + abs((int)c - 3);
          if (king.color == WHITE)
            score -= dist * 2; // Menos puntos por estar lejos del centro
          else
            score +=
                dist * 2; // Más puntos por estar lejos del centro para negras
        }
      }
    }
  }

  // Aplicar penalización por estar en jaque (utilizando las banderas
  // encontradas en la Pasada 1)
  if (white_king_in_check_flag)
    score += KING_SAFETY_PENALTY * 2;
  if (black_king_in_check_flag)
    score -= KING_SAFETY_PENALTY * 2;

  return score;
}
