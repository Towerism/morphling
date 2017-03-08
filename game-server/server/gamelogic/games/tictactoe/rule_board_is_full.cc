#include "rule_board_is_full.h"

#include "tictactoe_model.h"

namespace Morphling::Gamelogic::Tictactoe {

  bool Rule_board_is_full::check(Model* model) {
    bool result = false;
    auto tictactoe_model = dynamic_cast<Tictactoe_model*>(model);
    if (tictactoe_model != nullptr) {
      result = check_real(tictactoe_model);
    }
    return result;
  }

  bool Rule_board_is_full::check_real(Tictactoe_model* model) {
    bool result = true;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (model->get_element(i, j) == nullptr)
          return false;
      }
    }
    return result;
  }

  std::string Rule_board_is_full::get_message() const {
    return "The board is full!";
  }
}
