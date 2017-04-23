#include <gmock/gmock.h>

#include <games/tictactwo/action_move_grid.h>
#include <games/tictactwo/tictactwo_model.h>

using namespace Morphling::Gamelogic::Tictactwo;
using Morphling::Gamelogic::Point2D;

void pass_turns(Tictactwo_model& model, int turns) {
  for (int i = 0; i < turns; ++i)
    model.to_next_player();
}

TEST(IllegalMoveGridActionTests, TooEarlyByManyTurns) {
  Tictactwo_model model({0, 0});
  pass_turns(model, 0);
  Action_move_grid action(Action_move_grid::Direction::Down);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooEarlyByOneTurn) {
  Tictactwo_model model({0, 0});
  pass_turns(model, 3);
  Action_move_grid action(Action_move_grid::Direction::Down);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooFarLeft) {
  Point2D grid_origin(0, 1);
  Tictactwo_model model(grid_origin);
  pass_turns(model, 4);
  Action_move_grid action(Action_move_grid::Direction::Left);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooFarRight) {
  Point2D grid_origin(2, 1);
  Tictactwo_model model(grid_origin);
  pass_turns(model, 4);
  Action_move_grid action(Action_move_grid::Direction::Right);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooFarUp) {
  Point2D grid_origin(1, 0);
  Tictactwo_model model(grid_origin);
  pass_turns(model, 4);
  Action_move_grid action(Action_move_grid::Direction::Up);

  EXPECT_FALSE(action.is_legal(&model));
}

TEST(IllegalMoveGridActionTests, TooFarDown) {
  Point2D grid_origin(1, 2);
  Tictactwo_model model(grid_origin);
  pass_turns(model, 4);
  Action_move_grid action(Action_move_grid::Direction::Down);

  EXPECT_FALSE(action.is_legal(&model));
}
