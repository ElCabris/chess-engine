#include "game.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

bool parse_move_input(const char *input, Move *move) {
  char trimmed[6];
  size_t len;
  char from_col, to_col, from_row, to_row;
  int f_row, t_row;

  if (!input || !move || strlen(input) < 4) {
    return false;
  }

  strncpy(trimmed, input, 5);
  trimmed[5] = '\0';

  len = strlen(trimmed);
  if (trimmed[len - 1] == '\n') {
    trimmed[len - 1] = '\0';
    len--;
  }

  if (len != 4) {
    return false;
  }

  from_col = (char)tolower((unsigned char)trimmed[0]);
  from_row = trimmed[1];
  to_col = (char)tolower((unsigned char)trimmed[2]);
  to_row = trimmed[3];

  if (from_col < 'a' || from_col > 'h' || to_col < 'a' || to_col > 'h' ||
      from_row < '1' || from_row > '8' || to_row < '1' || to_row > '8') {
    return false;
  }

  f_row = from_row - '0';
  t_row = to_row - '0';

  move->from.col = (unsigned int)(from_col - 'a');
  move->from.row = (unsigned int)(8 - f_row);
  move->to.col = (unsigned int)(to_col - 'a');
  move->to.row = (unsigned int)(8 - t_row);

  return true;
}
