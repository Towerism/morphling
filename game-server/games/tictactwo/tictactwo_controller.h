#pragma once

#include <gamelogic/controller/controller.h>

namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_controller : public Controller{
  public:
    Tictactwo_controller(Model* model) : Controller(model) {}

    virtual action_t parse_action(std::string action_string) override;
  protected:
    virtual void execute_action(action_t action) override;
  };
}
