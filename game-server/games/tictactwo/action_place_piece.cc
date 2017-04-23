#include "action_place_piece.h"

#include "tictactwo_model.h"
#include "tictactwo_player.h"

namespace Morphling::Gamelogic::Tictactwo {
  bool Action_place_piece::is_legal(Model* model) {
    auto tictactwo_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactwo_model != nullptr) {
      return is_location_available_and_inside_grid(tictactwo_model);
    }
    return false;
  }

  bool Action_place_piece::is_location_available_and_inside_grid(Tictactwo_model* model) {
    return is_location_available(model) && is_location_inside_grid(model);
  }

  bool Action_place_piece::is_location_available(Tictactwo_model* model) {
    return model->get_element(location) == nullptr;
  }

  bool Action_place_piece::is_location_inside_grid(Tictactwo_model* model) {
    auto grid_origin = model->get_grid_origin();
    auto grid_bound = grid_origin.get_translation({3, 3});
    return location >= grid_origin && location < grid_bound;
  }

  void Action_place_piece::execute(Model* model) {
    auto tictactwo_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactwo_model != nullptr) {
      place_piece(tictactwo_model);
    }
  }

  void Action_place_piece::place_piece(Tictactwo_model* model) {
    auto player = dynamic_cast<Tictactwo_player*>(model->get_current_player());
    if (player != nullptr) {
      auto gamepiece = player->get_gamepiece();
      model->set_element(location, gamepiece);
    }
  }
}
