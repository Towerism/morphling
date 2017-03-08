#pragma once

#include <gamelogic/controller/game_engine.h>
#include <gamelogic/model/model.h>
#include <gamelogic/model/player.h>

#include "tictactoe_player.h"

namespace Morphling::Gamelogic::Tictactoe {

  class Tictactoe_engine : public Game_engine {
  public:
    Controller* initialize() override;
  };
}

