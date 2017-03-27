#include <gmock/gmock.h>

#include <gamelogic/games/tictactwo/action_move_grid.h>
#include <gamelogic/games/tictactwo/tictactwo_model.h>

using namespace Morphling::Gamelogic::Tictactwo;
using Morphling::Gamelogic::Point2D;

class LegalMoveGridActionTests : public ::testing::Test {
public:
  LegalMoveGridActionTests()
    : grid_origin(2, 2),
      model(new Tictactwo_model(grid_origin)) {
    pass_turns(4);
  }

  ~LegalMoveGridActionTests() {
    delete model;
  }

  void pass_turns(int turns) {
    for (int i = 0; i < turns; ++i)
      model->to_next_player();
  }

  Point2D grid_origin;
  Tictactwo_model* model;
};

TEST_F(LegalMoveGridActionTests, MoveGridUp) {
  Action_move_grid action(Action_move_grid::Direction::Up);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(2, 1),model->get_grid_origin());
}

TEST_F(LegalMoveGridActionTests, MoveGridDown) {
  Action_move_grid action(Action_move_grid::Direction::Down);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(2, 3),model->get_grid_origin());
}

TEST_F(LegalMoveGridActionTests, MoveGridLeft) {
  Action_move_grid action(Action_move_grid::Direction::Left);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(1, 2),model->get_grid_origin());
}

TEST_F(LegalMoveGridActionTests, MoveGridRight) {
  Action_move_grid action(Action_move_grid::Direction::Right);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(3, 2),model->get_grid_origin());
}

TEST_F(LegalMoveGridActionTests, MoveGridUpLeft) {
  Action_move_grid action(Action_move_grid::Direction::Up_left);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(1, 1),model->get_grid_origin());
}

TEST_F(LegalMoveGridActionTests, MoveGridUpRight) {
  Action_move_grid action(Action_move_grid::Direction::Up_right);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(3, 1),model->get_grid_origin());
}

TEST_F(LegalMoveGridActionTests, MoveGridDownLeft) {
  Action_move_grid action(Action_move_grid::Direction::Down_left);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(1, 3),model->get_grid_origin());
}

TEST_F(LegalMoveGridActionTests, MoveGridDownRight) {
  Action_move_grid action(Action_move_grid::Direction::Down_right);

  EXPECT_TRUE(action.is_legal(model));
  action.execute(model);

  EXPECT_EQ(Point2D(3, 3),model->get_grid_origin());
}
