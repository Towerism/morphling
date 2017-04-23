#include "tictactoe_model.h"

namespace Morphling::Gamelogic::Tictactoe {

  bool Tictactoe_model::is_game_over() {
    return has_alignment.check(this) || board_is_full.check(this);
  }

  bool Tictactoe_model::check_win_condition() {
    return has_alignment.check(this);
  }
}
