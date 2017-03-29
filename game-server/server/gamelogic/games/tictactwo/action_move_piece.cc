#include "action_move_piece.h"

#include "tictactwo_model.h"
#include "tictactwo_player.h"

namespace Morphling::Gamelogic::Tictactwo {
  bool Action_move_piece::is_legal(Model* model) {
    auto tictactwo_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactwo_model != nullptr) {
      return verify_turn_and_locations(tictactwo_model);
    }
    return false;
  }

  bool Action_move_piece::verify_turn_and_locations(Tictactwo_model* model) {
    return fourth_turn_has_passed(model)
      && place_piece.is_legal(model)
      && from_location_belongs_to_current_player(model);
  }

  bool Action_move_piece::fourth_turn_has_passed(Tictactwo_model* model) {
    return model->get_turn() > 4;
  }

  bool Action_move_piece::from_location_belongs_to_current_player(Tictactwo_model* model) {
    auto player = dynamic_cast<Tictactwo_player*>(model->get_current_player());
    auto piece = dynamic_cast<Gamepiece*>(model->get_element(from_location).get());
    if (player == nullptr || piece == nullptr)
      return false;
    return player->owns_gamepiece(piece);
  }

  void Action_move_piece::execute(Model* model) {
    auto tictactwo_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactwo_model != nullptr) {
      move_piece(tictactwo_model);
    }
  }

  void Action_move_piece::move_piece(Tictactwo_model* model) {
    auto player = dynamic_cast<Tictactwo_player*>(model->get_current_player());
    if (player != nullptr) {
      model->set_element(from_location, nullptr);
      place_piece.execute(model);
    }
  }
}
