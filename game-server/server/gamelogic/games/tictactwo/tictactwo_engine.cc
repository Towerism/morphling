#include "tictactwo_engine.h"

#include "tictactwo_controller.h"
#include "tictactwo_model.h"
#include "tictactwo_player.h"

namespace Morphling::Gamelogic::Tictactwo {
  Controller* Tictactwo_engine::initialize(std::string p1_name, std::string p2_name) {
    auto p1 = new Tictactwo_player(p1_name, new Gamepiece('X'));
    auto p2 = new Tictactwo_player(p2_name, new Gamepiece('O'));

    Tictactwo_model* model = new Tictactwo_model({0, 0});
    Tictactwo_controller* controller = new Tictactwo_controller(model);

    model->set_player_one(p1);
    model->set_player_two(p2);

    return controller;
  }
}
