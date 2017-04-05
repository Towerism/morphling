#include <gtest/gtest.h>

#include <gamelogic/games/tictactwo/tictactwo_model.h>
#include <gamelogic/games/tictactwo/action_place_piece.h>
#include <gamelogic/games/tictactwo/tictactwo_player.h>
#include <gamelogic/games/tictactwo/rule_has_alignment.h>

using namespace Morphling::Gamelogic::Tictactwo;

class HasAlignmentRuleMovingGridTests : public ::testing::Test {
public:
  HasAlignmentRuleMovingGridTests()
    : piece_x(new Gamepiece('X')) {
  }

  Gamepiece* piece_x;
};

TEST_F(HasAlignmentRuleMovingGridTests, CheckReturnsFalseWhenPiecesAlignButAreOutsideOfGrid) {
  Tictactwo_player player("playerone", piece_x);
  Tictactwo_model model({ 1, 1 });
  model.set_player_one(&player);
  Rule_has_alignment has_alignment;
  Action_place_piece({1, 0}).execute(&model);
  Action_place_piece({1, 1}).execute(&model);
  Action_place_piece({1, 2}).execute(&model);

  EXPECT_FALSE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleMovingGridTests, CheckReturnsTrueWhenPiecesAlignInsideOfGrid) {
  Tictactwo_player player("playerone", piece_x);
  Tictactwo_model model({ 1, 1 });
  model.set_player_one(&player);
  Rule_has_alignment has_alignment;
  Action_place_piece({1, 1}).execute(&model);
  Action_place_piece({1, 2}).execute(&model);
  Action_place_piece({1, 3}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}