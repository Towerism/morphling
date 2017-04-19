#pragma once

#include <memory>

#include <gamelogic/model/player.h>
#include <gamelogic/model/gameobject/game_object.h>

namespace Morphling::Gamelogic::Tictactwo {

  class Tictactwo_player : public Player {
  public:
    Tictactwo_player(std::string name, Game_object* gamepiece)
      : Player(name), gamepiece(gamepiece) {}

    auto get_gamepiece() { return gamepiece; }

    bool owns_gamepiece(Game_object* piece) const {
      return gamepiece->get_representation() == piece->get_representation();
    }

  private:
    std::shared_ptr<Game_object> gamepiece;
  };

}
