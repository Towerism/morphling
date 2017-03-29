#include <gtest/gtest.h>

#include <gamelogic/games/tictactwo/tictactwo_model.h>
#include <gamelogic/games/tictactwo/action_place_piece.h>
#include <gamelogic/games/tictactwo/tictactwo_player.h>
#include <gamelogic/games/tictactwo/rule_board_is_full.h>

using namespace Morphling::Gamelogic::Tictactwo;

class BoardIsFullRuleTests : public ::testing::Test {
public:
  BoardIsFullRuleTests()
    : piece_x(new Gamepiece('X')), player("playerone", piece_x), model({ 2, 2 }) {
    model.set_player_one(&player);
  }

  Gamepiece* piece_x;
  Tictactwo_player player;
  Tictactwo_model model;
};

TEST_F(BoardIsFullRuleTests, CheckReturnsFalseForEmptyBoard) {
  Rule_board_is_full board_is_full;
  EXPECT_FALSE(board_is_full.check(&model));
}

TEST_F(BoardIsFullRuleTests, CheckReturnsFalseForSparseBoard) {
  Rule_board_is_full board_is_full;
  Action_place_piece({2, 2}).execute(&model);
  Action_place_piece({2, 3}).execute(&model);
  EXPECT_FALSE(board_is_full.check(&model));
}

TEST_F(BoardIsFullRuleTests, CheckReturnsTrueForFullBoard) {
  Rule_board_is_full board_is_full;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      Action_place_piece({i, j}).execute(&model);
    }
  }
  EXPECT_TRUE(board_is_full.check(&model));
}
