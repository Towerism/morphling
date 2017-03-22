#pragma once

#include <gamelogic/model/model.h>
#include "controller.h"

namespace Morphling::Gamelogic {
  class Game_engine
  {
  public:
    virtual ~Game_engine() = default;

    virtual Controller* initialize(std::string p1_name, std::string p2_name) = 0;

    bool is_game_over() { return model->is_game_over(); }

    Player* get_winner() { return model->get_winner(); };

  protected:
    void set_model(Model* model) { this->model = model; }
    void set_controller(Controller* controller) { this->controller = controller; }

    Model* get_model() { return model; }

  private:
    Model* model;
    Controller* controller;
  };

}
