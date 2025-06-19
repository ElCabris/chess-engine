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
  int count;
} MoveList;

typedef struct {
  Piece **elements;
  unsigned int rows, cols;
} Board;

typedef enum { WHITE_TURN, BLACK_TURN } Turn;

Board *create_board(unsigned int rows, unsigned int cols);
void init_standard_board(Board *board);
void print_board(const Board *board);
void free_board(Board *board);
bool is_on_board(const Board *board, Position pos);
void free_move_list(MoveList *list);
