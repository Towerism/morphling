#pragma once

#include <gamelogic/model/model.h>
#include <gamelogic/model/gameobject/board2d.h>
#include "rule_has_alignment.h"
#include "rule_board_is_full.h"

namespace Morphling::Gamelogic::Tictactoe {
  class Tictactoe_model : public Model {
  public:
    Tictactoe_model() : board(3, 3) {}

    Board2D::gamepiece_t get_element(Point2D coordinates) {
      return board.get(coordinates);
    }

    void set_element(Point2D coordinates, Board2D::gamepiece_t game_object) {
      board.set(coordinates, game_object);
    }

    bool is_game_over() override;

    bool check_win_condition() override;

  private:
    Board2D board;
    Rule_board_is_full board_is_full;
    Rule_has_alignment has_alignment;
  };
}
