#pragma once

#include <memory>

#include <gamelogic/model/player.h>
#include <gamelogic/model/gameobject/game_object.h>

namespace Morphling::Gamelogic::Tictactoe {

  class Tictactoe_player : public Player {
  public:
    Tictactoe_player(std::string name, Game_object* gamepiece)
      : Player(name), gamepiece(gamepiece) {}

    auto get_gamepiece() { return gamepiece; }

  private:
    std::shared_ptr<Game_object> gamepiece;
  };

}
