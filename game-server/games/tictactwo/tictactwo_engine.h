#pragma once

#include <gamelogic/controller/game_engine.h>

namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_engine : public Game_engine {
  public:
    Controller* initialize(std::string p1_name, std::string p2_name) override;
  };
}

