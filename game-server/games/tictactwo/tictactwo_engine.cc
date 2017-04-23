#include "tictactwo_engine.h"

#include <gamelogic/view/board2d_to_strings_mapper.h>

#include "tictactwo_controller.h"
#include "tictactwo_model.h"
#include "tictactwo_player.h"

namespace Morphling::Gamelogic::Tictactwo {
  Controller* Tictactwo_engine::initialize(std::string p1_name, std::string p2_name) {
    auto p1 = new Tictactwo_player(p1_name, new Game_object('X'));
    auto p2 = new Tictactwo_player(p2_name, new Game_object('O'));

    token_to_url_mapper.add_url('X', "/assets/X.png");
    token_to_url_mapper.add_url('O', "/assets/O.png");

    Tictactwo_model* model = new Tictactwo_model({0, 0});
    Tictactwo_controller* controller = new Tictactwo_controller(model);

    controller->set_model_serializer(new Board2D_to_strings_mapper(model->get_board()));

    model->set_player_one(p1);
    model->set_player_two(p2);

    return controller;
  }
}
