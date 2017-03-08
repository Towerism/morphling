#pragma once

#include <memory>
#include <model/player.h>

#include "gamepiece.h"

namespace Morphling::Gamelogic::Tictactoe {

  class Tictactoe_player : public Player {
  public:
    Tictactoe_player(std::string name, Gamepiece* gamepiece)
      : Player(name), gamepiece(gamepiece) {}

    auto get_gamepiece() { return gamepiece; }

  private:
    std::shared_ptr<Gamepiece> gamepiece;
  };

}
