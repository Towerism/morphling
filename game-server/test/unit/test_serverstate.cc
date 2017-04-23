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
    std::string player1_hash = "player1_hash";
    std::string player2_hash = "player2_hash";

    ServerStateTest(): serverstate(new Tictactoe::Tictactoe_engine()), fb{"morphling-50028"} {
        // Initialize the firebase with a game
        fire_err fe = fb.write_json(".json",json(R"(
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
                "states": {
                    "validgame": {
                        "0": {
                            "0": "_,_,_",
                            "1": "_,x,_",
                            "2": "_,_,_"
                        }
                    }
                }
            }
        )"_json));
        EXPECT_EQ(fe.res_code,CURLE_OK);
    }
};

TEST_F(ServerStateTest, CheckValidGame) {
    auto game = serverstate.get_game(validgame);
    ASSERT_NE(game,nullptr);
    EXPECT_EQ(game->gameid,validgame);
    EXPECT_EQ(game->player1.name,player1_hash);
}

TEST_F(ServerStateTest, CheckInvalidGameid) {
    auto game = serverstate.get_game("badgameid");
    ASSERT_EQ(game,nullptr);
}

TEST_F(ServerStateTest, CheckEndGameWhite) {
    auto game = serverstate.get_game(validgame);
    ASSERT_NE(game,nullptr);
    EXPECT_EQ(game->gameid,validgame);
    EXPECT_EQ(game->player1.name,player1_hash);

    // check the player's score before end_game
    fire_err fe = fb.get_json("players/"+player1_hash+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["score"],0);

    serverstate.end_game(validgame,PlayerSide::White);
    fe = fb.get_json("games/"+validgame+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["score"],player1);
}

TEST_F(ServerStateTest, CheckEndGameBlack) {
    auto game = serverstate.get_game(validgame);
    ASSERT_NE(game,nullptr);
    EXPECT_EQ(game->gameid,validgame);
    EXPECT_EQ(game->player2.name,player2_hash);

    // check the player's score before end_game
    fire_err fe = fb.get_json("players/"+player2_hash+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    EXPECT_EQ(fe.res_json["score"],0);

    serverstate.end_game(validgame,PlayerSide::Black);
    fe = fb.get_json("games/"+validgame+".json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
}

TEST_F(ServerStateTest, CheckUpdatedState) {
    auto game = serverstate.get_game(validgame);
    ASSERT_NE(game,nullptr);
    EXPECT_EQ(game->gameid,validgame);

    auto board_state = std::vector<std::string>{"_,_,_","o,x,_","_,_,_"};
    serverstate.send_move(validgame,board_state);
    fire_err fe = fb.get_json("states/"+validgame+"/1.json");
    ASSERT_EQ(fe.res_code,CURLE_OK);
    ASSERT_EQ(fe.res_json[0],board_state[0]);
}
