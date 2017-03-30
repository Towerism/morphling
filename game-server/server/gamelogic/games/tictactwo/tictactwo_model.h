#pragma once

#include <gamelogic/model/model.h>
#include <gamelogic/model/point2d.h>
#include <gamelogic/model/gameobject/board2d.h>

#include "rule_has_alignment.h"
#include "rule_board_is_full.h"

namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_model : public Model {
  public:
    Tictactwo_model(Point2D grid_origin) : board(5, 5), grid_origin(grid_origin) {}

    Board2D::gamepiece_t get_element(Point2D coordinates) {
      return board.get(coordinates);
    }

    void set_element(Point2D coordinates, Board2D::gamepiece_t game_object) {
      board.set(coordinates, game_object);
    }

    bool is_game_over() override;
    bool check_win_condition() override;
    Game_result get_result() override;

    const Point2D& get_grid_origin() const { return grid_origin; }

    void set_grid_origin(const Point2D& new_grid_origin) {
      grid_origin = new_grid_origin;
    }

    const Player* get_player_owning_gamepiece(Board2D::gamepiece_t game_object);

  private:
    Board2D board;
    Point2D grid_origin;
    const Player* winner;

    Rule_board_is_full board_is_full;
    Rule_has_alignment has_alignment;
  };
}
