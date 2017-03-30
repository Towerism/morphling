#include "tictactwo_model.h"
#include "tictactwo_player.h"

namespace Morphling::Gamelogic::Tictactwo {
  bool Tictactwo_model::is_game_over() {
    return check_win_condition() || board_is_full.check(this);
  }

  bool Tictactwo_model::check_win_condition() {
    bool result = has_alignment.check(this);
    winner = has_alignment.get_causal_player();
    return result;
  }

  Game_result Tictactwo_model::get_result() {
    if (!is_game_over())
      return Game_result::Undetermined;
    if (check_win_condition()) {
      if (winner == get_player_one())
        return Game_result::Player_one;
      else
        return Game_result::Player_two;
    }
    return Game_result::Tie;
  }

  const Player* Tictactwo_model::get_player_owning_gamepiece(Board2D::gamepiece_t game_object) {
    auto gamepiece = dynamic_cast<Gamepiece*>(game_object.get());
    auto player_one = dynamic_cast<const Tictactwo_player*>(get_player_one());
    auto player_two = dynamic_cast<const Tictactwo_player*>(get_player_two());
    if (!gamepiece || !player_one || !player_two)
      return nullptr;
    if (player_one->owns_gamepiece(gamepiece))
      return player_one;
    else
      return player_two;
  }
}
