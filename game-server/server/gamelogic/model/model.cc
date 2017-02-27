#include "model.h"

namespace Morphling::Gamelogic {
  Player* Model::to_next_player() {
    if (current_player == player_one) {
      current_player = player_two;
    } else {
      current_player = player_one;
    }
    return current_player;
  }
}
