#include <gtest/gtest.h>

#include <gamelogic/games/tictactwo/tictactwo_model.h>
#include <gamelogic/games/tictactwo/action_place_piece.h>
#include <gamelogic/games/tictactwo/tictactwo_player.h>

using namespace Morphling::Gamelogic::Tictactwo;

class PlacePieceActionTests : public ::testing::Test {
public:
  PlacePieceActionTests()
    : piece_x(new Gamepiece('X')), player("playerone", piece_x), model({ 2, 2 }) {
    model.set_player_one(&player);
  }

  Gamepiece* piece_x;
  Tictactwo_player player;
  Tictactwo_model model;
};

TEST_F(PlacePieceActionTests, LegalToPlaceInsideGrid) {
  Action_place_piece action({ 2, 3 });

  EXPECT_TRUE(action.is_legal(&model));
}

TEST_F(PlacePieceActionTests, IllegalToPlacePastOriginOfGrid) {
  Action_place_piece action({ 1, 1 });

  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(PlacePieceActionTests, IllegalToPlacePastEndOfGrid) {
  Action_place_piece action({ 5, 5 });

  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(PlacePieceActionTests, IllegalToPlacePastRightOfGrid) {
  Action_place_piece action({ 5, 2 });

  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(PlacePieceActionTests, IllegalToPlacePastLeftOfGrid) {
  Action_place_piece action({ 2, 5 });

  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(PlacePieceActionTests, IllegalToPlaceAboveGrid) {
  Action_place_piece action({ 2, 1 });

  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(PlacePieceActionTests, IllegalToPlaceBelowGrid) {
  Action_place_piece action({ 2, 5 });

  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(PlacePieceActionTests, PlacingPieceOccupiesTheSpace) {
  Action_place_piece action({ 2, 3 });

  action.execute(&model);

  ASSERT_NE(nullptr, model.get_element({2, 3}));
  EXPECT_TRUE(model.get_element({2, 3})->equals(piece_x));
}

TEST_F(PlacePieceActionTests, IllegalToPlaceOnOccupiedSpace) {
  Action_place_piece action({ 2, 3 });

  action.execute(&model);

  EXPECT_FALSE(action.is_legal(&model));
}
