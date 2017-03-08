#pragma once

namespace Morphling::Gamelogic {
  class Game_object {
  public:
    Game_object() = default;
    virtual ~Game_object() = default;

    virtual bool equals(Game_object* other) const {
      return this == other;
    }
  };
}
