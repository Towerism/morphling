#include <gmock/gmock.h>

#include <gamelogic/games/tictactwo/action_move_grid.h>
#include <gamelogic/games/tictactwo/tictactwo_model.h>

using namespace Morphling::Gamelogic::Tictactwo;
using Morphling::Gamelogic::Point2D;

TEST(IllegalMoveGridActionTests, TooFarLeft) {
  Point2D grid_origin(0, 1);
  Tictactwo_model model(grid_origin);
  Action_move_grid action(Action_move_grid::Direction::Left);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooFarRight) {
  Point2D grid_origin(4, 1);
  Tictactwo_model model(grid_origin);
  Action_move_grid action(Action_move_grid::Direction::Right);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooFarUp) {
  Point2D grid_origin(1, 0);
  Tictactwo_model model(grid_origin);
  Action_move_grid action(Action_move_grid::Direction::Up);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooFarDown) {
  Point2D grid_origin(1, 4);
  Tictactwo_model model(grid_origin);
  Action_move_grid action(Action_move_grid::Direction::Down);

  EXPECT_FALSE(action.is_legal(&model));
}
