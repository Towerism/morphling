#include <gamelogic/model/model.h>

#include <gtest/gtest.h>

using namespace Morphling::Gamelogic;

class ModelImpl : public Model {
  bool is_game_over() override { return false; }
  bool check_win_condition() override { return false; }
};

class ModelTest : public ::testing::Test {
public:
  ModelTest() {
    model = new ModelImpl;
    player_one = new Player("p1");
    player_two = new Player("p2");

    model->set_player_one(player_one);
    model->set_player_two(player_two);
  }
  ~ModelTest() {
    delete model;
    delete player_one;
    delete player_two;
  }
protected:
  Model* model;
  Player* player_one;
  Player* player_two;
};

TEST_F(ModelTest, CurrentPlayerIsInitiallyPlayerOne) {
  EXPECT_EQ(player_one, model->get_current_player());
}

TEST_F(ModelTest, ToNextPlayerResultsInPlayerTwosTurn) {
  EXPECT_EQ(player_two, model->to_next_player());
}

TEST_F(ModelTest, ToNextPlayerTwiceResultsInPlayerOnesTurn) {
  model->to_next_player();
  model->to_next_player();
  EXPECT_EQ(player_one, model->get_current_player());
}

