#include "tictactoe_engine.h"

#include "tictactoe_controller.h"
#include "tictactoe_model.h"

namespace Morphling::Gamelogic::Tictactoe {
  Controller* Tictactoe_engine::initialize() {
    auto p1 = new Tictactoe_player("Player 1", new Gamepiece('X'));
    auto p2 = new Tictactoe_player("Player 2", new Gamepiece('O'));

    Tictactoe_model* model = new Tictactoe_model;
    Tictactoe_controller* controller = new Tictactoe_controller(model);

    model->set_player_one(p1);
    model->set_player_two(p2);

    set_model(model);
    set_controller(controller);
    
    return controller;
  }
}
