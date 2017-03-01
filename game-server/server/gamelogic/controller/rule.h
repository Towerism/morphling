#pragma once

#include <model/model.h>

namespace Morphling::Gamelogic {
  class Rule
  {
  public:
    virtual ~Rule() = default;

    virtual bool check(Model* model) = 0;
  };

}
