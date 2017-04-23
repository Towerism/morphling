#include <gtest/gtest.h>
#include <serverstate/server_state.h>
#include <database/firebase.h>
#include <games/tictactoe/tictactoe_engine.h>

#include <string>

using namespace Morphling::Database;
using namespace Morphling::Gamelogic;
using namespace Morphling::ServerState;

class ServerStateTest : public ::testing::Test {
protected:
    Server_state serverstate;
    firebase fb;
    
    std::string validgame = "validgame";
    std::string player1 = "player1";
    std::string player2 = "player2";

    ServerStateTest(): serverstate(new Tictactoe::Tictactoe_engine()), fb{"morphling-50028"} {
        // Initialize the firebase with a game
        fire_err fe = fb.write_json(".json",json(R"(
            {
                "games": {
                    "validgame": {
                        "player1": "player1",
                        "player2": "player2"
                    }
                },
                "players": {
                    "player1": {
                        "name": "player1",
                        "score": 0
                    },
                    "player2": {
                        "name": "player2",
                        "score": 0
                    }
                }
            }
        )"_json));
        EXPECT_EQ(fe.res_code,CURLE_OK);
    }
};

TEST_F(ServerStateTest, CheckValidGame) {
    auto game = serverstate.get_game(validgame,player1);
    ASSERT_NE(game,nullptr);
    EXPECT_EQ(game->gameid,validgame);
    EXPECT_EQ(game->player1.name,player1);
}

TEST_F(ServerStateTest, CheckInvalidGameid) {
    auto game = serverstate.get_game("badgameid",player1);
    ASSERT_EQ(game,nullptr);
}

TEST_F(ServerStateTest, CheckEndGameWhite) {
    auto game = serverstate.get_game(validgame,player1);
    ASSERT_NE(game,nullptr);
    EXPECT_EQ(game->gameid,validgame);
    EXPECT_EQ(game->player1.name,player1);

    // check the player's score before end_game
    fire_err fe = fb.get_json("players/"+player1+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["score"],0);

    serverstate.end_game(validgame,PlayerSide::White);
    fe = fb.get_json("games/"+validgame+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["score"],player1);
}

TEST_F(ServerStateTest, CheckEndGameBlack) {
    auto game = serverstate.get_game(validgame,player2);
    ASSERT_NE(game,nullptr);
    EXPECT_EQ(game->gameid,validgame);
    EXPECT_EQ(game->player2.name,player2);

    // check the player's score before end_game
    fire_err fe = fb.get_json("players/"+player2+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["score"],0);

    serverstate.end_game(validgame,PlayerSide::Black);
    fe = fb.get_json("games/"+validgame+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
}
