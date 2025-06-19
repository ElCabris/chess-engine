#include "chess_engine.h"
#include <limits.h>

static int alphabeta(Game *game, int depth, int alpha, int beta,
                     bool maximizingPlayer) {
  ColorPiece color;
  MoveList moves;
  int bestValue;
  unsigned int i;
  if (depth == 0 || is_checkmate(game) || is_stalemate(game)) {
    return evaluate_board(game->board);
  }

  color = maximizingPlayer ? WHITE : BLACK;
  moves = get_all_legal_moves(game->board, color);

  if (moves.count == 0) {
    free_move_list(&moves);
    return evaluate_board(game->board);
  }

  if (maximizingPlayer) {
    bestValue = INT_MIN;
    for (i = 0; i < moves.count; ++i) {
      Board *copy = copy_board(game->board);
      Game next_state;
      int val;

      apply_move(copy, moves.moves[i]);
      next_state = *game;
      next_state.board = copy;

      val = alphabeta(&next_state, depth - 1, alpha, beta, false);
      if (val > bestValue)
        bestValue = val;
      if (val > alpha)
        alpha = val;
      free_board(copy);
      if (beta <= alpha)
        break;
    }
  } else {
    bestValue = INT_MAX;
    for (i = 0; i < moves.count; ++i) {
      Board *copy = copy_board(game->board);
      Game next_state;
      int val;

      apply_move(copy, moves.moves[i]);
      next_state = *game;
      next_state.board = copy;

      val = alphabeta(&next_state, depth - 1, alpha, beta, true);
      if (val < bestValue)
        bestValue = val;
      if (val < beta)
        beta = val;
      free_board(copy);
      if (beta <= alpha)
        break;
    }
  }

  free_move_list(&moves);
  return bestValue;
}

Move get_best_move(const Game *game, int depth) {
  MoveList moves;
  Move best_move;
  int best_value;
  unsigned int i;

  moves = get_all_legal_moves(game->board,
                              game->turn == WHITE_TURN ? WHITE : BLACK);
  best_move = (Move){{0, 0}, {0, 0}};
  best_value = (game->turn == WHITE_TURN) ? INT_MIN : INT_MAX;

  for (i = 0; i < moves.count; ++i) {
    Board *copy = copy_board(game->board);
    Game next_state;
    int value;

    apply_move(copy, moves.moves[i]);
    next_state = *game;
    next_state.board = copy;
    next_state.turn = (game->turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN;

    value = alphabeta(&next_state, depth - 1, INT_MIN, INT_MAX,
                      game->turn != WHITE_TURN);

    if ((game->turn == WHITE_TURN && value > best_value) ||
        (game->turn == BLACK_TURN && value < best_value)) {
      best_value = value;
      best_move = moves.moves[i];
    }

    free_board(copy);
  }

  free_move_list(&moves);
  return best_move;
}
