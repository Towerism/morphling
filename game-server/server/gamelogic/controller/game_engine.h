#pragma once

#include <gamelogic/model/model.h>
#include <gamelogic/view/token_to_url_mapper.h>
#include "controller.h"

namespace Morphling::Gamelogic {
  class Game_engine
  {
  public:
    virtual ~Game_engine() = default;

    virtual Controller* initialize(std::string p1_name, std::string p2_name) = 0;

    std::string map_token_to_url(char token) const { return token_to_url_mapper.get_url(token); }

  protected:
    Token_to_url_mapper token_to_url_mapper;
  };
}
