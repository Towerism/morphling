#pragma once

#include <gamelogic/model/model.h>
#include "controller.h"

namespace Morphling::Gamelogic {
  class Game_engine
  {
  public:
    virtual ~Game_engine() = default;

    virtual Controller* initialize(std::string p1_name, std::string p2_name) = 0;
  };
}
