#include "controller.h"

#include <memory>

namespace Morphling::Gamelogic {
  bool Controller::receive_action(std::string action_string) {
    auto action = std::unique_ptr<Action>(parse_action(action_string));
    if (!action || !action->is_legal(model))
      return false;
    action->execute(model);
    return true;
  }
}
