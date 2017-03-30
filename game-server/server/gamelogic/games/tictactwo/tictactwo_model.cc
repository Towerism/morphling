#include "tictactwo_model.h"

namespace Morphling::Gamelogic::Tictactwo {
  bool Tictactwo_model::is_game_over() {
    return check_win_condition() || board_is_full.check(this);
  }

  bool Tictactwo_model::check_win_condition() {
    return has_alignment.check(this);
  }
}
