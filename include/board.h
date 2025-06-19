#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h>

typedef enum { KING, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, NO_TYPE } TypePiece;

typedef enum { WHITE, BLACK, NO_COLOR } ColorPiece;

typedef struct {
  TypePiece type;
  ColorPiece color;
  bool has_moved;
} Piece;

typedef struct {
  unsigned int row, col;
} Position;

typedef struct {
  Position from, to;
} Move;

typedef struct {
  Move *moves;
  unsigned int count;
} MoveList;

typedef struct {
  Piece **elements;
  unsigned int rows, cols;
} Board;

typedef enum { WHITE_TURN, BLACK_TURN } Turn;

typedef struct {
  Board *board;
  Turn turn;
  Move last_move;
  bool game_over;
  ColorPiece winner;
} Game;

Board *create_board(unsigned int rows, unsigned int cols);
void init_standard_board(Board *board);
void print_board(const Board *board);
void free_board(Board *board);
bool is_on_board(const Board *board, Position pos);
void free_move_list(MoveList *list);
Board *copy_board(const Board *board);

void apply_move(Board *board, Move move);

MoveList get_valid_moves_pawn(const Board *board, Position from);
MoveList get_valid_moves_rook(const Board *board, Position from);
MoveList get_valid_moves_bishop(const Board *board, Position from);
MoveList get_valid_moves_knight(const Board *board, Position from);
MoveList get_valid_moves_queen(const Board *board, Position from);
MoveList get_valid_moves_king(const Board *board, Position from);

bool is_in_check(const Board *board, ColorPiece color);
bool is_move_legal(const Board *board, Move move, ColorPiece color);

MoveList filter_legal_moves(const Board *board, MoveList raw_moves,
                            ColorPiece color);

MoveList get_all_legal_moves(const Board *board, ColorPiece color);

void make_move(Game *game, Move move);
bool is_checkmate(const Game *game);
bool is_stalemate(const Game *game);

#endif
