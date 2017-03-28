#pragma once

#include <gamelogic/model/model.h>
#include <gamelogic/model/point2d.h>
#include <gamelogic/model/gameobject/board2d.h>

namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_model : public Model {
  public:
    Tictactwo_model(Point2D grid_origin) : board(5, 5), grid_origin(grid_origin) {}

    Board2D::gamepiece_t get_element(Point2D coordinates) {
      return board.get(coordinates);
    }

    void set_element(Point2D coordinates, Board2D::gamepiece_t game_object) {
      board.set(coordinates, game_object);
    }

    bool is_game_over() override;
    bool check_win_condition() override;

    const Point2D& get_grid_origin() const { return grid_origin; }

    void set_grid_origin(const Point2D& new_grid_origin) {
      grid_origin = new_grid_origin;
    }

  private:
    Board2D board;
    Point2D grid_origin;
  };
}
