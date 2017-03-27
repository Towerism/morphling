#include "gameobject/board2d.h"

namespace Morphling::Gamelogic {
  class Element_accessor {
  public:
    virtual Board2D::gamepiece_t get_element(int i, int j) = 0;
    virtual void set_element(int i, int j, Board2D::gamepiece_t game_object) = 0;

    virtual ~Element_accessor() = default;
  };
}
