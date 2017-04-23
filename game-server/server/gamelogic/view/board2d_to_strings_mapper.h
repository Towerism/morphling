#pragma once

#include "model_to_strings_mapper.h"

#include <gamelogic/model/gameobject/board2d.h>

namespace Morphling::Gamelogic {
  class Board2D_to_strings_mapper : public Model_to_strings_mapper {
  public:
    Board2D_to_strings_mapper(const Board2D& board) : board(board) {}

    strings_t get_strings() override;
  private:
    const Board2D& board;
  };
}
