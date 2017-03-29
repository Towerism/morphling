#pragma once

#include <gamelogic/model/gameobject/game_object.h>

namespace Morphling::Gamelogic::Tictactwo {
  class Gamepiece : public Game_object {
  public:
    Gamepiece(char representation) : representation(representation) {}

    bool equals(Game_object* other) const override {
      bool equal = false;
      auto gamepiece = dynamic_cast<Gamepiece*>(other);
      if (gamepiece != nullptr)
        equal = representation == gamepiece->representation;
      return equal;
    }

    char get_representation() const { return representation; };

  private:
    char representation;
  };
}
