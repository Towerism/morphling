#include <gtest/gtest.h>

#include <gamelogic/games/tictactwo/action_move_piece.h>
#include <gamelogic/games/tictactwo/action_place_piece.h>
#include <gamelogic/games/tictactwo/tictactwo_model.h>
#include <gamelogic/games/tictactwo/tictactwo_player.h>

using namespace Morphling::Gamelogic::Tictactwo;
using Morphling::Gamelogic::Point2D;

class MovePieceActionTests : public ::testing::Test {
public:
  MovePieceActionTests()
      : piece_x(new Gamepiece('X')), piece_o(new Gamepiece('O')),
        player1("playerone", piece_x), player2("playertwo", piece_o),
        model({2, 2}), from_location(2, 2), to_location(3, 3),
        other_player_piece_location(1, 1) {
    model.set_player_one(&player1);
    model.set_player_two(&player2);
    Action_place_piece(from_location).execute(&model);
    model.to_next_player();
    Action_place_piece(other_player_piece_location).execute(&model);
    model.to_next_player();
  }

  void pass_turns(int turns) {
    for (int i = 0; i < turns; ++i)
      model.to_next_player();
  }

  Gamepiece *piece_x;
  Gamepiece *piece_o;
  Tictactwo_player player1;
  Tictactwo_player player2;
  Tictactwo_model model;
  Point2D from_location;
  Point2D to_location;
  Point2D other_player_piece_location;
};

TEST_F(MovePieceActionTests, IllegalToMovePieceOneTurnBeforeFifthTurn) {
  Action_move_piece action(from_location, to_location);
  pass_turns(1);
  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(MovePieceActionTests, IllegalToMovePieceBeforeFifthTurn) {
  Action_move_piece action(from_location, to_location);
  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(MovePieceActionTests, IllegalToMovePieceToLocationOutsideGrid) {
  Action_move_piece action(from_location, {0, 0});
  pass_turns(2);
  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(MovePieceActionTests, IllegalToMovePieceFromLocationOutsideGrid) {
  Action_move_piece action({0, 0}, to_location);
  pass_turns(2);
  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(MovePieceActionTests, IllegalToMovePieceToOccupiedSpace) {
  Action_move_piece action(from_location, other_player_piece_location);
  pass_turns(2);
  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(MovePieceActionTests, IllegalToMoveNonExistentPiece) {
  from_location.translate({2, 2});
  Action_move_piece action(from_location, to_location);
  pass_turns(2);
  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(MovePieceActionTests, IllegalToMovePieceOfOtherPlayer) {
  Action_move_piece action(other_player_piece_location, to_location);
  pass_turns(2);
  EXPECT_FALSE(action.is_legal(&model));
}

TEST_F(MovePieceActionTests, MovingPieceFreesUpTheOldSpace) {
  Action_move_piece action(from_location, to_location);
  pass_turns(2);
  action.execute(&model);
  EXPECT_EQ(nullptr, model.get_element(from_location));
}

TEST_F(MovePieceActionTests, MovingPieceOccupiesTheNewSpace) {
  Action_move_piece action(from_location, to_location);
  pass_turns(2);
  action.execute(&model);
  EXPECT_TRUE(model.get_element(to_location)->equals(piece_x));
}
