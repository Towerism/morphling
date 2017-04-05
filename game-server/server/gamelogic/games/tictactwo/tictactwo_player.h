#pragma once

#include <memory>

#include <gamelogic/model/player.h>

#include "gamepiece.h"

namespace Morphling::Gamelogic::Tictactwo {

  class Tictactwo_player : public Player {
  public:
    Tictactwo_player(std::string name, Gamepiece* gamepiece)
      : Player(name), gamepiece(gamepiece) {}

    auto get_gamepiece() { return gamepiece; }

    bool owns_gamepiece(Gamepiece* piece) const {
      return gamepiece->get_representation() == piece->get_representation();
    }

  private:
    std::shared_ptr<Gamepiece> gamepiece;
  };

}