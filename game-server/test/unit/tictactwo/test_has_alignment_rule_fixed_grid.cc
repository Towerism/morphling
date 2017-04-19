#include <gtest/gtest.h>

#include <gamelogic/games/tictactwo/tictactwo_model.h>
#include <gamelogic/games/tictactwo/action_place_piece.h>
#include <gamelogic/games/tictactwo/tictactwo_player.h>
#include <gamelogic/games/tictactwo/rule_has_alignment.h>

using namespace Morphling::Gamelogic::Tictactwo;

using Morphling::Gamelogic::Game_object;

class HasAlignmentRuleFixedGridTests : public ::testing::Test {
public:
  HasAlignmentRuleFixedGridTests()
    : piece_x(new Game_object('X')), player("playerone", piece_x), model({ 0, 0 }) {
    model.set_player_one(&player);
  }

  Game_object* piece_x;
  Tictactwo_player player;
  Tictactwo_model model;
};

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsFalseWhenPiecesDontAlign) {
  Rule_has_alignment has_alignment;
  Action_place_piece({0, 0}).execute(&model);
  Action_place_piece({2, 2}).execute(&model);
  Action_place_piece({0, 2}).execute(&model);
  Action_place_piece({2, 0}).execute(&model);

  EXPECT_FALSE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForHorizontalTop) {
  Rule_has_alignment has_alignment;
  Action_place_piece({0, 0}).execute(&model);
  Action_place_piece({1, 0}).execute(&model);
  Action_place_piece({2, 0}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForHorizontalMiddle) {
  Rule_has_alignment has_alignment;
  Action_place_piece({0, 1}).execute(&model);
  Action_place_piece({1, 1}).execute(&model);
  Action_place_piece({2, 1}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForHorizontalBottom) {
  Rule_has_alignment has_alignment;
  Action_place_piece({0, 2}).execute(&model);
  Action_place_piece({1, 2}).execute(&model);
  Action_place_piece({2, 2}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}


TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForVerticalLeft) {
  Rule_has_alignment has_alignment;
  Action_place_piece({0, 0}).execute(&model);
  Action_place_piece({0, 1}).execute(&model);
  Action_place_piece({0, 2}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForVerticalRight) {
  Rule_has_alignment has_alignment;
  Action_place_piece({2, 0}).execute(&model);
  Action_place_piece({2, 1}).execute(&model);
  Action_place_piece({2, 2}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForVerticalMiddle) {
  Rule_has_alignment has_alignment;
  Action_place_piece({1, 0}).execute(&model);
  Action_place_piece({1, 1}).execute(&model);
  Action_place_piece({1, 2}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForDiagonalTopToBottom) {
  Rule_has_alignment has_alignment;
  Action_place_piece({0, 0}).execute(&model);
  Action_place_piece({1, 1}).execute(&model);
  Action_place_piece({2, 2}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}

TEST_F(HasAlignmentRuleFixedGridTests, CheckReturnsTrueForDiagonalBottomToTop) {
  Rule_has_alignment has_alignment;
  Action_place_piece({0, 2}).execute(&model);
  Action_place_piece({1, 1}).execute(&model);
  Action_place_piece({2, 0}).execute(&model);

  EXPECT_TRUE(has_alignment.check(&model));
}
