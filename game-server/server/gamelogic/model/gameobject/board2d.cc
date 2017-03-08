#include "board2d.h"

namespace Morphling::Gamelogic {
  Board2D::gamepiece_t Board2D::get(int i, int j) {
    gamepiece_t gamepiece;
    if (check_dimensions(i, j)) {
      gamepiece = gamepiece_t(board[i * width + j]);
    }
    return gamepiece;
  }

  void Board2D::set(int i, int j, gamepiece_t game_object) {
    if (check_dimensions(i, j)) {
      board[i * width + j] = game_object;
    }
  }

  bool Board2D::check_dimensions(int i, int j) {
    return i > -1 && j > -1 && i < height && j < width;
  }
}
