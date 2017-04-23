#include <gtest/gtest.h>
#include <database/firebase.h>

using namespace Morphling::Database;

class FirebaseTest : public ::testing::Test {
protected:
    FirebaseTest(): f{"morphling-50028"} {
        // Initialize the firebase with a game
        fire_err fe = f.write_json(".json",json(R"(
            {
                "games": {
                    "validgame": {
                        "player1": "player1_hash",
                        "player2": "player2_hash"
                    }
                },
                "players": {
                    "player1_hash": {
                        "name": "player1_name",
                        "score": 0
                    },
                    "player2_hash": {
                        "name": "player2_name",
                        "score": 0
                    }
                },
                "test": {
                    "Test": 15
                }
            }
        )"_json));
        EXPECT_EQ(fe.res_code,CURLE_OK);
    }

    firebase f;
};

TEST_F(FirebaseTest, WriteCheck) {
    fire_err fe = f.write_json("test.json",json::parse("{\"Test\":15}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],15);
}

TEST_F(FirebaseTest, WriteMultiCheck) {
    fire_err fe = f.write_json("test.json",json::parse("{\"Test\":15}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],15);

    fe = f.write_json("test.json",json::parse("{\"Test\":99}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],99);
}

TEST_F(FirebaseTest, GetCheck) {
    fire_err fe = f.get_json("test.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json["Test"].is_number());
    ASSERT_EQ(fe.res_json["Test"],15);
}

TEST_F(FirebaseTest, GetShallowCheck) {
    fire_err fe = f.get_json("test/Test.json",true);
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_TRUE(fe.res_json.is_number());
    ASSERT_EQ(fe.res_json,15);
}

TEST_F(FirebaseTest, GetValidGame) {
    fire_err fe = f.get_json("games/validgame.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["player1"],"player1_hash");
    EXPECT_EQ(fe.res_json["player2"],"player2_hash");
    EXPECT_EQ(fe.res_json["score"],nullptr);
}

TEST_F(FirebaseTest, GetInvalidGame) {
    fire_err fe = f.get_json("games/invalidgame.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json,nullptr);
}

TEST_F(FirebaseTest, GetPlayers) {
    fire_err fe = f.get_json("players.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["player1_hash"]["name"],"player1_name");
    EXPECT_EQ(fe.res_json["player2_hash"]["name"],"player2_name");
}

