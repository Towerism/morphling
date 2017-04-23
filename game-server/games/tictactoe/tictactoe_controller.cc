#include "tictactoe_controller.h"

#include <sstream>

#include "action_mark.h"
#include "tictactoe_model.h"

namespace Morphling::Gamelogic::Tictactoe {
  Controller::action_t Tictactoe_controller::parse_action(std::string action_string) {
    std::istringstream iss(action_string);
    int x, y;
    iss >> x >> y;
    if (!iss)
      return action_t(nullptr);
    return action_t(new Action_mark(x, y));
  }

  void Tictactoe_controller::execute_action(action_t action) {
    if (action->is_legal(model)) {
      action->execute(model);
      if (!model->is_game_over())
        model->to_next_player();
    }
  }
}
