#include <gtest/gtest.h>

#include <gamelogic/view/token_to_url_mapper.h>

using Morphling::Gamelogic::Token_to_url_mapper;

class TokenToUrlMapperTests : public ::testing::Test {
public:
  Token_to_url_mapper target;
};

TEST_F(TokenToUrlMapperTests, AddUrl) {
  char token = 'x';
  std::string url = "theurl";

  target.add_url(token, url);

  EXPECT_EQ(url, target.get_url(token));
}

TEST_F(TokenToUrlMapperTests, AddManyUrls) {
  char x = 'x';
  char y = 'y';
  std::string urlx = "theurl";
  std::string urly = "anotherurl";

  target.add_url(x, urlx);
  target.add_url(y, urly);

  EXPECT_EQ(urlx, target.get_url(x));
  EXPECT_EQ(urly, target.get_url(y));
}

TEST_F(TokenToUrlMapperTests, GetUnmappedUrlThrowsException) {
  EXPECT_THROW(target.get_url('x'), Token_to_url_mapper::Token_not_mapped);
}

TEST_F(TokenToUrlMapperTests, AddAlreadyMappedUrlThrowsException) {
  char token = 'x';
  target.add_url(token, "theurl");
  EXPECT_THROW(target.add_url(token, "anotherurl"), Token_to_url_mapper::Token_already_mapped);
}
