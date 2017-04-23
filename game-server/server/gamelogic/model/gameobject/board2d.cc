#include "board2d.h"

namespace Morphling::Gamelogic {
  Board2D::gamepiece_t Board2D::get(Point2D coordinates) {
    gamepiece_t gamepiece;
    if (check_dimensions(coordinates)) {
      int i = coordinates.get_x();
      int j = coordinates.get_y();
      gamepiece = gamepiece_t(board[i * width + j]);
    }
    return gamepiece;
  }

  const Board2D::gamepiece_t Board2D::get(Point2D coordinates) const {
    gamepiece_t gamepiece;
    if (check_dimensions(coordinates)) {
      int i = coordinates.get_x();
      int j = coordinates.get_y();
      gamepiece = gamepiece_t(board[i * width + j]);
    }
    return gamepiece;
  }

  void Board2D::set(Point2D coordinates, gamepiece_t game_object) {
    if (check_dimensions(coordinates)) {
      int i = coordinates.get_x();
      int j = coordinates.get_y();
      board[i * width + j] = game_object;
    }
  }

  bool Board2D::check_dimensions(Point2D coordinates) const {
    int i = coordinates.get_x();
    int j = coordinates.get_y();
    return i > -1 && j > -1 && i < height && j < width;
  }
}
