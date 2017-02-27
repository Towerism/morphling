#include "game_engine.h"

namespace Morphling::Gamelogic {
  void Game_engine::play() {
    initialize();
    while(!model->is_game_over()) {
      controller->update();
    }
    end();
  }
}
