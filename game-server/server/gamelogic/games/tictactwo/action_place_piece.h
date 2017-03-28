#pragma once

#include <gamelogic/controller/action.h>
#include <gamelogic/model/point2d.h>

namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_model;
  
  class Action_place_piece : public Action {
  public:
    Action_place_piece(Point2D location) : location(location) {}

    bool is_legal(Model* model) override;

    void execute(Model* model) override;

  private:
    Point2D location;

    bool is_location_available_and_inside_grid(Tictactwo_model* model);
    bool is_location_available(Tictactwo_model* model);
    bool is_location_inside_grid(Tictactwo_model* model);
    void place_piece(Tictactwo_model* model);
  };
}
