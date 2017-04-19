#include <gtest/gtest.h>
#include <database/firebase.h>

using namespace Morphling::Database;

TEST(FIREBASETest, WriteCheck) {
    firebase f("morphling-50028");
    fire_err fe = f.write_json("test.json",json::parse("{\"Test\":15}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],15);
}

TEST(FIREBASETest, WriteMultiCheck) {
    firebase f("morphling-50028");
    fire_err fe = f.write_json("test.json",json::parse("{\"Test\":15}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],15);

    fe = f.write_json("test.json",json::parse("{\"Test\":99}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],99);
}

TEST(FIREBASETest, GetCheck) {
    firebase f("morphling-50028");
    fire_err fe = f.get_json("test.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],99);
}

TEST(FIREBASETest, GetShallowCheck) {
    firebase f("morphling-50028");
    fire_err fe = f.get_json("test/Test.json",true);
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json.is_number());
    ASSERT_EQ(fe.res_json,99);
}

TEST(FIREBASETest, GetValidGame) {
    firebase f("morphling-50028");
    fire_err fe = f.get_json("games/validgame.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["player1"],"player1");
    EXPECT_EQ(fe.res_json["player2"],"player2");
    EXPECT_EQ(fe.res_json["score"],nullptr);
}

TEST(FIREBASETest, GetInvalidGame) {
    firebase f("morphling-50028");
    fire_err fe = f.get_json("games/invalidgame.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json,nullptr);
}

