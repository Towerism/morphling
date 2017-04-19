#include <gtest/gtest.h>
#include <networking/gcp_server.h>
#include <networking/gcp_client.h>
#include <gamelogic/games/tictactoe/tictactoe_engine.h>

using namespace Morphling::Networking;
using namespace Morphling::Gamelogic;

class TicTacToePlayTest : public ::testing::Test {
protected:
    TicTacToePlayTest(): server(new Tictactoe::Tictactoe_engine()) {
        EXPECT_TRUE(server.start(0));
        EXPECT_TRUE(server.is_running());
        // Connect clients
        EXPECT_TRUE(server.is_running());
        EXPECT_TRUE(client1.connect("127.0.0.1",server.get_port()));
        EXPECT_TRUE(client2.connect("127.0.0.1",server.get_port()));

        // Send Auth for client1
        EXPECT_TRUE(client1.connected());
        auto res = client1.send_auth("validgame","player1");
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);

        // Send Auth for client2
        EXPECT_TRUE(client2.connected());
        res = client2.send_auth("validgame","player2");
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
    }

    GCPServer server;
    GCPClient client1;
    GCPClient client2;
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
};

// TEST_F(TicTacToePlayTest, InvalidMoveWhiteTwice) {
//     // White makes an invalid move
//     auto res = client1.send_move("4 3");
//     res = client1.recv_move();
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Error);
//     EXPECT_EQ(std::get<1>(res),"MOVE:INVALID(3 3)");

//     // Make the invalid move again
//     res = client1.send_move("3 4");
//     res = client1.recv_move();
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Error);
//     EXPECT_EQ(std::get<1>(res),"MOVE:INVALID(3 4)");

//     // Disconnect
//     client1.disconnect();
//     EXPECT_FALSE(client1.connected());
//     client2.disconnect();
//     EXPECT_FALSE(client2.connected());
//     server.stop();
//     EXPECT_FALSE(server.is_running());
// }

TEST_F(TicTacToePlayTest, TwoPlayersWhiteWin) {
    // Send Move (White)
    auto res = client1.send_move(moves1[0]);
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

TEST_F(TicTacToePlayTest, TwoPlayersBlackWin) {
    // Send Move (White)
    auto res = client1.send_move(moves2[0]);
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    // Alternating moves
    recieve_and_make_move(client2,1,moves2);
    recieve_and_make_move(client1,2,moves2);
    recieve_and_make_move(client2,3,moves2);
    recieve_and_make_move(client1,4,moves2);
    recieve_and_make_move(client2,5,moves2);
    // White lose
    res = client1.recv_move();
    EXPECT_TRUE(client1.is_gameover());
    EXPECT_FALSE(client1.is_winner());
    // Black win
    res = client2.recv_move();
    EXPECT_TRUE(client2.is_gameover());
    EXPECT_TRUE(client2.is_winner());

    // Disconnect
    client1.disconnect();
    EXPECT_FALSE(client1.connected());
    client2.disconnect();
    EXPECT_FALSE(client2.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
}