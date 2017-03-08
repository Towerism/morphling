#pragma once

#include <model/player.h>

#include <string>

namespace Morphling::Gamelogic {
  class Player {
  public:
    Player(std::string name) : name(name) {}
    virtual ~Player() = default;

    std::string get_name() const { return name; }

  private:
    std::string name;
  };
}
