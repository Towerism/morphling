#include <gtest/gtest.h>

#include <gamelogic/controller/controller.h>
#include <gamelogic/games/tictactoe/tictactoe_engine.h>

using namespace Morphling::Gamelogic;

class TictactoeTests : public ::testing::Test {
public:
  TictactoeTests() : engine() {
    p1_name = "Player One";
    p2_name = "Player Two";

    controller = engine.initialize(p1_name, p2_name);
  }

  Tictactoe::Tictactoe_engine engine;
  Controller* controller;

  std::string p1_name, p2_name;
};

TEST_F(TictactoeTests, PlayerOneWins) {
  EXPECT_TRUE(controller->receive_action("0 0"));
  EXPECT_TRUE(controller->receive_action("0 1"));
  EXPECT_TRUE(controller->receive_action("1 0"));
  EXPECT_TRUE(controller->receive_action("0 2"));
  EXPECT_TRUE(controller->receive_action("2 0"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_one, controller->get_result());
}

TEST_F(TictactoeTests, PlayerTwoWins) {
  EXPECT_TRUE(controller->receive_action("0 0"));
  EXPECT_TRUE(controller->receive_action("1 0"));
  EXPECT_TRUE(controller->receive_action("2 0"));
  EXPECT_TRUE(controller->receive_action("1 1"));
  EXPECT_TRUE(controller->receive_action("2 1"));
  EXPECT_TRUE(controller->receive_action("1 2"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Player_two, controller->get_result());
}

TEST_F(TictactoeTests, TieGame) {
  EXPECT_TRUE(controller->receive_action("0 0"));
  EXPECT_TRUE(controller->receive_action("1 1"));
  EXPECT_TRUE(controller->receive_action("0 1"));
  EXPECT_TRUE(controller->receive_action("0 2"));
  EXPECT_TRUE(controller->receive_action("2 0"));
  EXPECT_TRUE(controller->receive_action("1 0"));
  EXPECT_TRUE(controller->receive_action("1 2"));
  EXPECT_TRUE(controller->receive_action("2 1"));
  EXPECT_TRUE(controller->receive_action("2 2"));

  EXPECT_TRUE(controller->is_game_over());
  EXPECT_EQ(Game_result::Tie, controller->get_result());
}

TEST_F(TictactoeTests, UndeterminedGame) {
  EXPECT_TRUE(controller->receive_action("0 0"));
  EXPECT_TRUE(controller->receive_action("1 1"));
  EXPECT_TRUE(controller->receive_action("0 1"));
  EXPECT_TRUE(controller->receive_action("0 2"));
  EXPECT_TRUE(controller->receive_action("2 0"));
  EXPECT_TRUE(controller->receive_action("1 0"));

  EXPECT_FALSE(controller->is_game_over());
  EXPECT_EQ(Game_result::Undetermined, controller->get_result());
}
