#include "tictactwo_controller.h"

#include <sstream>

#include "action_move_grid.h"
#include "action_move_piece.h"
#include "action_place_piece.h"

namespace Morphling::Gamelogic::Tictactwo {
  // PLACE x y
  // MOVE x y x y
  // GRID direction (0-7)
  Controller::action_t Tictactwo_controller::parse_action(std::string action_string) {
    std::istringstream iss(action_string);
    Point2D from_location, to_location;
    int direction;
    std::string action_identifier;
    action_t action;
    iss >> action_identifier;
    if (action_identifier == "place") {
      iss >> to_location;
      action = action_t(new Action_place_piece(to_location));
    }
    else if (action_identifier == "move") {
      iss >> from_location >> to_location;
      action = action_t(new Action_move_piece(from_location, to_location));
    }
    else if (action_identifier == "grid") {
      iss >> direction;
      if (direction >= (int)Action_move_grid::Direction::NUM_DIRECTIONS)
        return nullptr;
      auto dir = static_cast<Action_move_grid::Direction>(direction);
      action = action_t(new Action_move_grid(dir));
    }
    if (!iss)
      return nullptr;
    return action;
  }

  void Tictactwo_controller::execute_action(action_t action) {
    if (action->is_legal(model)) {
      action->execute(model);
      if (!model->is_game_over())
        model->to_next_player();
    }
  }
}
