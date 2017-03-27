#include "model.h"

namespace Morphling::Gamelogic {
  Game_result Model::get_result() {
    if (!is_game_over())
      return Game_result::Undetermined;
    if (check_win_condition()) {
      if (current_player == player_one)
        return Game_result::Player_one;
      return Game_result::Player_two;
    }
    return Game_result::Tie;
  }

  Player* Model::to_next_player() {
    ++turn;
    if (current_player == player_one) {
      current_player = player_two;
    } else {
      current_player = player_one;
    }
    return current_player;
  }
}
