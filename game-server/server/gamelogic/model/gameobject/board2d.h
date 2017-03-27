#pragma once

#include <memory>
#include <vector>

#include "game_object.h"

namespace Morphling::Gamelogic {
  class Board2D : public Game_object {
  public:
    typedef std::shared_ptr<Game_object> gamepiece_t;
    typedef std::vector<gamepiece_t> board_t;

    Board2D(int height, int width) : height(height), width(width), board(height * width) {}
    virtual ~Board2D() = default;

    gamepiece_t get(int i, int j);
    void set(int i, int j, Game_object* game_object) { set(i, j, gamepiece_t(game_object)); }
    void set(int i, int j, gamepiece_t game_object);

    int get_width() const { return width; }
    int get_height() const { return height; }

  private:
    int height, width;
    board_t board;

    bool check_dimensions(int i, int j);
  };

}
