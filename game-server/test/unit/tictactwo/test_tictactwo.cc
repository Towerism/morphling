#include <gtest/gtest.h>

#include <gamelogic/games/tictactwo/tictactwo_engine.h>

using namespace Morphling::Gamelogic;

class TictactwoTests : public ::testing::Test {
public:
  TictactwoTests() {
    controller = engine.initialize("player1", "player2");
  }

  Tictactwo::Tictactwo_engine engine;
  Controller* controller;
};

TEST_F(TictactwoTests, PlayerOneWinsPlacementActionsOnly) {
  EXPECT_TRUE(controller->receive_action("place 0 0"));
  EXPECT_TRUE(controller->receive_action("place 0 1"));
  EXPECT_TRUE(controller->receive_action("place 1 0"));
  EXPECT_TRUE(controller->receive_action("place 0 2"));
  EXPECT_TRUE(controller->receive_action("place 2 0"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_one, controller->get_result());
}

TEST_F(TictactwoTests, PlayerTwoWinsPlacementActionsOnly) {
  EXPECT_TRUE(controller->receive_action("place 0 0"));
  EXPECT_TRUE(controller->receive_action("place 1 0"));
  EXPECT_TRUE(controller->receive_action("place 0 1"));
  EXPECT_TRUE(controller->receive_action("place 1 1"));
  EXPECT_TRUE(controller->receive_action("place 2 2"));
  EXPECT_TRUE(controller->receive_action("place 1 2"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_two, controller->get_result());
}

TEST_F(TictactwoTests, PlayerOneWinsIncludingMovementActions) {
  EXPECT_TRUE(controller->receive_action("place 0 0"));
  EXPECT_TRUE(controller->receive_action("place 1 0"));
  EXPECT_TRUE(controller->receive_action("place 2 1"));
  EXPECT_TRUE(controller->receive_action("place 1 1"));
  EXPECT_TRUE(controller->receive_action("place 2 2"));
  EXPECT_TRUE(controller->receive_action("move 1 1 1 2"));
  EXPECT_TRUE(controller->receive_action("move 0 0 2 0"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_one, controller->get_result());
}

TEST_F(TictactwoTests, PlayerOneWinsIncludingGridActions) {
  EXPECT_TRUE(controller->receive_action("place 0 0"));
  EXPECT_TRUE(controller->receive_action("place 1 0"));
  EXPECT_TRUE(controller->receive_action("place 0 1"));
  EXPECT_TRUE(controller->receive_action("place 1 1"));
  EXPECT_TRUE(controller->receive_action("grid 3"));
  EXPECT_TRUE(controller->receive_action("grid 2"));
  EXPECT_TRUE(controller->receive_action("place 2 2"));
  EXPECT_TRUE(controller->receive_action("place 1 2"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_two, controller->get_result());
}

TEST_F(TictactwoTests, PlayerOneWinsIncludingAllActions) {
  EXPECT_TRUE(controller->receive_action("place 0 0"));
  EXPECT_TRUE(controller->receive_action("place 1 0"));
  EXPECT_TRUE(controller->receive_action("place 0 1"));
  EXPECT_TRUE(controller->receive_action("place 1 1"));
  EXPECT_TRUE(controller->receive_action("grid 3"));
  EXPECT_TRUE(controller->receive_action("grid 2"));
  EXPECT_TRUE(controller->receive_action("move 0 1 1 2"));
  EXPECT_TRUE(controller->receive_action("move 1 1 2 2"));
  EXPECT_TRUE(controller->receive_action("move 1 2 0 1"));
  EXPECT_TRUE(controller->receive_action("move 2 2 1 1"));
  EXPECT_TRUE(controller->receive_action("place 2 2"));
  EXPECT_TRUE(controller->receive_action("place 1 2"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_two, controller->get_result());
}

TEST_F(TictactwoTests, PlayerTwoWinsOnPlayerOnesTurn) {
  EXPECT_TRUE(controller->receive_action("place 0 0")); // 1
  EXPECT_TRUE(controller->receive_action("place 1 0")); // 2
  EXPECT_TRUE(controller->receive_action("place 0 1")); // 1
  EXPECT_TRUE(controller->receive_action("place 1 1")); // 2
  EXPECT_TRUE(controller->receive_action("grid 1")); // 1
  EXPECT_TRUE(controller->receive_action("place 1 2")); // 2
  EXPECT_TRUE(controller->receive_action("grid 1")); // 1
  EXPECT_TRUE(controller->receive_action("grid 0")); // 2
  EXPECT_TRUE(controller->receive_action("grid 0")); // 1

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_two, controller->get_result());
}
