#pragma once

namespace Morphling::Gamelogic {
  class Game_object {
  public:
    Game_object(char representation) : representation(representation) {}
    Game_object() : Game_object(' ') {}
    virtual ~Game_object() = default;

    bool equals(Game_object* other) const {
      bool equal = false;
      auto gamepiece = dynamic_cast<Game_object*>(other);
      if (gamepiece != nullptr)
        equal = representation == gamepiece->representation;
      return equal;
    }

    char get_representation() const { return representation; };

  private:
    char representation;
  };
}
