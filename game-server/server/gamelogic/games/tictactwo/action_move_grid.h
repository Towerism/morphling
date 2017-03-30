#pragma once

#include <gamelogic/controller/action.h>
#include <gamelogic/model/point2d.h>

namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_model;

  class Action_move_grid : public Action {
  public:
    enum class Direction : int {
      Up = 0, Down, Left, Right,
      Up_left, Up_right,
      Down_left, Down_right, NUM_DIRECTIONS
    };

    Action_move_grid(Direction direction);

    bool is_legal(Model* model) override;
    void execute(Model* model) override;

  private:
    Direction direction;

    bool is_direction_valid(Tictactwo_model* model);

    Point2D new_grid_origin_from_direction(Tictactwo_model* model);

    void move_grid(Tictactwo_model* model);
  };
}
