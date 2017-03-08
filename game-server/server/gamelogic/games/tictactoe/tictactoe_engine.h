#pragma once

#include <controller/game_engine.h>
#include <model/model.h>
#include <model/player.h>

#include "tictactoe_player.h"

namespace Morphling::Gamelogic::Tictactoe {

  class Tictactoe_engine : public Game_engine {
  public:
    Controller* initialize() override;
  };
}

