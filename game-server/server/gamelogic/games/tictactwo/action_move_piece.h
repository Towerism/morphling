#pragma once

#include <gamelogic/controller/action.h>
#include <gamelogic/model/point2d.h>
#include "action_place_piece.h"

namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_model;

  class Action_move_piece : public Action {
  public:
    Action_move_piece(Point2D from_location, Point2D to_location)
      : from_location(from_location), place_piece(to_location) {}

    bool is_legal(Model* model) override;

    void execute(Model* model) override;

  private:
    Point2D from_location;
    Action_place_piece place_piece;

    bool verify_turn_and_locations(Tictactwo_model* model);
    bool fourth_turn_has_passed(Tictactwo_model* model);
    bool from_location_belongs_to_current_player(Tictactwo_model* model);
    bool is_from_location_inside_grid(Tictactwo_model* model);
    void move_piece(Tictactwo_model* model);
  };
}
