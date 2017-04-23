#pragma once

#include <gamelogic/model/model.h>
#include <gamelogic/view/token_to_url_mapper.h>
#include <gamelogic/view/model_to_strings_mapper.h>
#include "controller.h"

namespace Morphling::Gamelogic {
  class Game_engine
  {
  public:
    virtual ~Game_engine() { if (model_to_strings_mapper) delete model_to_strings_mapper; }

    virtual Controller* initialize(std::string p1_name, std::string p2_name) = 0;

    std::string map_token_to_url(char token) const { return token_to_url_mapper.get_url(token); }
    Model_to_strings_mapper::strings_t map_model_to_strings() { return model_to_strings_mapper->get_strings(); }

  protected:
    Token_to_url_mapper token_to_url_mapper;
    Model_to_strings_mapper* model_to_strings_mapper;
  };
}
