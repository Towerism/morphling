#pragma once

#include <gamelogic/controller/game_engine.h>
#include <gamelogic/model/model.h>
#include <gamelogic/model/player.h>

#include "tictactoe_player.h"

namespace Morphling::Gamelogic::Tictactoe {

  class Tictactoe_engine : public Game_engine {
  public:
    Controller* initialize(std::string p1_name, std::string p2_name) override;

    bool is_game_over() const { return model->is_game_over(); }
    Player* get_winner() const { return model->get_winner(); }

  protected:
    void set_controller(Controller* controller) { this->controller = controller; }
    void set_model(Model* model) { this->model = model; }

  private:
    Model* model;
    Controller* controller;
  };
}

