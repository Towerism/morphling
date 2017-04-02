#include "tictactoe_engine.h"

#include "tictactoe_controller.h"
#include "tictactoe_model.h"

namespace Morphling::Gamelogic::Tictactoe {
  Controller* Tictactoe_engine::initialize(std::string p1_name, std::string p2_name) {
    auto p1 = new Tictactoe_player(p1_name, new Gamepiece('X'));
    auto p2 = new Tictactoe_player(p2_name, new Gamepiece('O'));

    Tictactoe_model* model = new Tictactoe_model;
    Tictactoe_controller* controller = new Tictactoe_controller(model);

    model->set_player_one(p1);
    model->set_player_two(p2);

    return controller;
  }
}
