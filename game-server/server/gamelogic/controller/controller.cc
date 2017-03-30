#include "controller.h"

#include <memory>

namespace Morphling::Gamelogic {
  bool Controller::receive_action(std::string action_string) {
    auto action = parse_action(action_string);
    if ((!action || !action->is_legal(model)) && !model->is_game_over())
      return false;
    execute_action(action);
    return true;
  }
}
