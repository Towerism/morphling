#pragma once

#include <memory>
#include <string>

#include <gamelogic/model/model.h>
#include <gamelogic/model/player.h>
#include "action.h"

namespace Morphling::Gamelogic {
  class Controller {
  public:
    typedef std::shared_ptr<Action> action_t;

    Controller(Model* model) : model(model) {}
    virtual ~Controller() = default;

    virtual action_t parse_action(std::string action_string) = 0;

    // Returns true if action was received successfully
    // Returns false otherwise.
    // Action can be recieved successfully if the action_string is
    // malformed or the parsed action is illegal.
    bool receive_action(std::string action_string);

    const Player* get_current_player() const { return model->get_current_player(); }
    Player* get_current_player() { return model->get_current_player(); }

  protected:
    virtual void execute_action(action_t action) = 0;
    Model* model;
  };

}
