#pragma once

#include <gamelogic/controller/controller.h>

namespace Morphling::Gamelogic::Tictactoe {
  class Tictactoe_model;

  class Tictactoe_controller : public Controller {
  public:
    Tictactoe_controller(Model* model) : Controller(model) {}

    action_t parse_action(std::string action_string) override;

  protected:
    void execute_action(action_t action) override;
  };
}

