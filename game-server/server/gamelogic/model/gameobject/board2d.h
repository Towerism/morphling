#pragma once

#include <memory>
#include <vector>

#include "game_object.h"
#include <gamelogic/model/point2d.h>

namespace Morphling::Gamelogic {
  class Board2D : public Game_object {
  public:
    typedef std::shared_ptr<Game_object> gamepiece_t;
    typedef std::vector<gamepiece_t> board_t;

    Board2D(int height, int width) : height(height), width(width), board(height * width) {}
    virtual ~Board2D() = default;

    gamepiece_t get(Point2D coordinates);
    void set(Point2D coordinates, Game_object* game_object) { set(coordinates, gamepiece_t(game_object)); }
    void set(Point2D coordinates, gamepiece_t game_object);

    int get_width() const { return width; }
    int get_height() const { return height; }

  private:
    int height, width;
    board_t board;

    bool check_dimensions(Point2D coordinates);
  };

}
