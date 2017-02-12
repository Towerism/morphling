#include <gtest/gtest.h>

class StubTest : public ::testing::Test {
public:
  bool test_helper() {
    return true;
  }
};

TEST_F(StubTest, TestHelperReturnsTrue) {
  EXPECT_TRUE(test_helper());
}
