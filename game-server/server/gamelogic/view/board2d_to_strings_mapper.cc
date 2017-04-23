#include "board2d_to_strings_mapper.h"

#include <sstream>

namespace Morphling::Gamelogic {
  Board2D_to_strings_mapper::strings_t Board2D_to_strings_mapper::get_strings() {
    std::ostringstream row;
    strings_t strings;
    for (int y = 0; y < board.get_height(); ++y) {
      for (int x = 0; x < board.get_width(); ++x) {
        auto game_object = board.get({x, y});
        if (game_object) {
          row << game_object->get_representation();
        } else {
          row << '_';
        }
        if (x < board.get_width() - 1) {
          row << ',';
        }
      }
      strings.push_back(row.str());
      row.str("");
      row.clear();
    }
    return strings;
  }
}
