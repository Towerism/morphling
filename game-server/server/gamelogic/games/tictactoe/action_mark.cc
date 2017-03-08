#include "action_mark.h"

#include "tictactoe_model.h"
#include "tictactoe_player.h"

namespace Morphling::Gamelogic::Tictactoe {

  bool Action_mark::is_legal(Model* model) {
    auto tictactoe_model = dynamic_cast<Tictactoe_model*>(model);
    if (tictactoe_model != nullptr) {
      return is_legal_real(tictactoe_model);
    }
    return false;
  }

  void Action_mark::execute(Model* model) {
    auto tictactoe_model = dynamic_cast<Tictactoe_model*>(model);
    if (tictactoe_model != nullptr) {
      execute(model);
    }
  }

  bool Action_mark::is_legal_real(Tictactoe_model* model) {
    return model->get_element(x, y) == nullptr;
  }

  void Action_mark::execute_real(Tictactoe_model* model) {
    auto player = dynamic_cast<Tictactoe_player*>(model->get_current_player());
    if (player != nullptr) {
      auto gamepiece = player->get_gamepiece();
      model->set_element(x, y, gamepiece);
    }
  }
}
