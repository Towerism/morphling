#include "tictactoe_engine.h"

#include <gamelogic/view/board2d_to_strings_mapper.h>

#include "tictactoe_controller.h"
#include "tictactoe_model.h"

namespace Morphling::Gamelogic::Tictactoe {
  Controller* Tictactoe_engine::initialize(std::string p1_name, std::string p2_name) {
    auto p1 = new Tictactoe_player(p1_name, new Game_object('X'));
    auto p2 = new Tictactoe_player(p2_name, new Game_object('O'));

    Tictactoe_model* model = new Tictactoe_model;
    Tictactoe_controller* controller = new Tictactoe_controller(model);

    controller->add_url('X', "/assets/X.png");
    controller->add_url('O', "/assets/O.png");

    controller->set_model_serializer(new Board2D_to_strings_mapper(model->get_board()));

    model->set_player_one(p1);
    model->set_player_two(p2);

    return controller;
  }
}
