#pragma once

#include <gamelogic/model/model.h>
#include <gamelogic/model/gameobject/board2d.h>
#include "rule_has_alignment.h"
#include "rule_board_is_full.h"

namespace Morphling::Gamelogic::Tictactoe {
  class Tictactoe_model : public Model {
  public:
    Tictactoe_model() : board(3, 3) {}

    Board2D::gamepiece_t get_element(int i, int j) {
      return board.get(i, j);
    }

    void set_element(int i, int j, Board2D::gamepiece_t game_object) {
      board.set(i, j, game_object);
    }

    bool is_game_over() override;

    Player* get_winner() override;

  private:
    Board2D board;
    Rule_board_is_full board_is_full;
    Rule_has_alignment has_alignment;
  };
}
