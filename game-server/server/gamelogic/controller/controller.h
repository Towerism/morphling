#pragma once

#include <string>

#include <model/model.h>
#include <model/player.h>
#include "action.h"

namespace Morphling::Gamelogic {
  class Controller
  {
  public:

    Controller(Model* model) : model(model) {}
    virtual ~Controller() = default;

    virtual void update() = 0;

    virtual Action* parse_action(std::string action_string) = 0;

    // Returns true if action was received successfully
    // Returns false otherwise.
    // Action can be recieved successfully if the action_string is
    // malformed or the parsed action is illegal.
    bool receive_action(std::string action_string);

    const Player* get_current_player() const { return model->get_current_player(); }
    Player* get_current_player() { return model->get_current_player(); }
  private:
    Model* model;
  };

}
