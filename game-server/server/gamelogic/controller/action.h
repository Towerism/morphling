#pragma once

#include <model/model.h>

namespace Morphling::Gamelogic {
  class Action {
  public:
    virtual bool is_legal(Model* model) = 0;
    virtual void execute(Model* model) = 0;

    virtual ~Action() = default;
  };
}
