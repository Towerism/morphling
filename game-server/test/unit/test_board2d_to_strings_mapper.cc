#include <gtest/gtest.h>

#include <gamelogic/view/board2d_to_strings_mapper.h>

using namespace Morphling::Gamelogic;

class Board2dToStringsMapperTests : public ::testing::Test {
public:
  Board2dToStringsMapperTests() : board(3, 3), target(board) {
    auto object_x = std::make_shared<Game_object>('x');
    auto object_y = std::make_shared<Game_object>('y');
    board.set({0, 0}, object_x);
    board.set({2, 0}, object_y);
    board.set({1, 2}, object_y);
  }
  Board2D board;
  Board2D_to_strings_mapper target;
};

TEST_F(Board2dToStringsMapperTests, GetStrings) {
  auto strings = target.get_strings();

  EXPECT_EQ("x,_,y", strings[0]);
  EXPECT_EQ("_,_,_", strings[1]);
  EXPECT_EQ("_,y,_", strings[2]);
}
