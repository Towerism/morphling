#include <gtest/gtest.h>
#include <networking/gcp_server.h>
#include <networking/gcp_client.h>
#include <games/tictactoe/tictactoe_engine.h>
#include <database/firebase.h>

#include <thread>

using namespace Morphling::Database;
using namespace Morphling::Networking;
using namespace Morphling::Gamelogic;

class ServerStressTest : public ::testing::Test {
protected:
    ServerStressTest():
        server(new Tictactoe::Tictactoe_engine(),"morphling-50028"),
        fb{"morphling-50028"}
    {
        // Initialize the firebase with a game
        fire_err fe = fb.write_json(".json",json(R"(
            {
                "games": {
                    "validgame1": {
                        "player1": "player1_hash",
                        "player2": "player2_hash"
                    },
                    "validgame2": {
                        "player1": "player3_hash",
                        "player2": "player4_hash"
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
                    },
                    "player3_hash": {
                        "name": "player3_name",
                        "score": 0
                    },
                    "player4_hash": {
                        "name": "player4_name",
                        "score": 0
                    }
                },
                "settings": {
                    "timeout": 20
                }
            }
        )"_json));
        EXPECT_EQ(fe.res_code,CURLE_OK);

        EXPECT_TRUE(server.start(0));
        EXPECT_TRUE(server.is_running());
    }

    GCPServer server;
    firebase fb;
    std::string moves1[6] =  {
        "0 0",
        "0 1",
        "1 0",
        "0 2",
        "2 0",
        ""
    };
    std::string moves2[6] =  {
        "0 1",
        "0 0",
        "0 2",
        "1 0",
        "1 1",
        "2 0"
    };

    void recieve_and_make_move(GCPClient& client, int move, std::string* moves) {
        auto res = client.recv_move();
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
        EXPECT_EQ(std::get<1>(res),moves[move-1]);
        res = client.send_move(moves[move]);
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    }

    void start_game(std::string game_str) {
        GCPClient client1;
        GCPClient client2;

        // Connect clients
        EXPECT_TRUE(server.is_running());
        EXPECT_TRUE(client1.connect("127.0.0.1",server.get_port()));
        EXPECT_TRUE(client2.connect("127.0.0.1",server.get_port()));

        // Send Auth for client1
        EXPECT_TRUE(client1.connected());
        auto res = client1.send_auth(game_str,"player1");
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);

        // Send Auth for client2
        EXPECT_TRUE(client2.connected());
        res = client2.send_auth(game_str,"player2");
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);

        // Recieve Auth for both clients
        res = client1.recv_auth();
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
        EXPECT_EQ(std::get<1>(res),"VALID");
        res = client2.recv_auth();
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
        EXPECT_EQ(std::get<1>(res),"VALID");

        // Verify player1 is White
        res = client1.recv_side();
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
        EXPECT_EQ(std::get<1>(res),"W");

        // Verify player2 is Black
        res = client2.recv_side();
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
        EXPECT_EQ(std::get<1>(res),"B");

        // Send Move (White)
        res = client1.send_move(moves1[0]);
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);

        // Alternating moves
        recieve_and_make_move(client2,1,moves1);
        recieve_and_make_move(client1,2,moves1);
        recieve_and_make_move(client2,3,moves1);
        recieve_and_make_move(client1,4,moves1);

        // White win
        res = client1.recv_move();
        EXPECT_TRUE(client1.is_gameover());
        EXPECT_TRUE(client1.is_winner());

        // Black lose
        res = client2.recv_move();
        EXPECT_TRUE(client2.is_gameover());
        EXPECT_FALSE(client2.is_winner());

        // Disconnect
        client1.disconnect();
        EXPECT_FALSE(client1.connected());
        client2.disconnect();
        EXPECT_FALSE(client2.connected());
        server.stop();
        EXPECT_FALSE(server.is_running());
    }

};

TEST_F(ServerStressTest, TwoGames) {
    std::thread game1([&]{
        start_game("validgame1");
    });
    std::thread game2([&]{
        start_game("validgame2");
    });
    game1.join();
    game2.join();
}
