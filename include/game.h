#ifndef GAME_H
#define GAME_H
#include <board.h>
bool parse_move_input(const char *input, Move *move);
bool get_player_move(const Game *game, Move *move_out);
void play_game(void);
#endif
