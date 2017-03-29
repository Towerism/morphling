#include "rule_board_is_full.h"

#include "tictactwo_model.h"

namespace Morphling::Gamelogic::Tictactwo {

  bool Rule_board_is_full::check(Model* model) {
    bool result = false;
    auto tictactoe_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactoe_model != nullptr) {
      result = check_board_is_full(tictactoe_model);
    }
    return result;
  }

  bool Rule_board_is_full::check_board_is_full(Tictactwo_model* model) {
    bool result = true;
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        if (model->get_element({i, j}) == nullptr)
          return false;
      }
    }
    return result;
  }

  std::string Rule_board_is_full::get_message() const {
    return "The board is full!";
  }
}
