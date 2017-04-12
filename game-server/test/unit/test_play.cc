#include <gtest/gtest.h>
#include <networking/gcp_server.h>
#include <networking/gcp_client.h>
#include <gamelogic/games/tictactoe/tictactoe_engine.h>

using namespace Morphling::Networking;
using namespace Morphling::Gamelogic;

class PlayTest : public ::testing::Test {
protected:
    PlayTest(): server(new Tictactoe::Tictactoe_engine()) {
        EXPECT_TRUE(server.start(0));
        EXPECT_TRUE(server.is_running());
    }

    GCPServer server;
    GCPClient client1;
    GCPClient client2;
    std::string moves[5] =  {
        "0 0",
        "0 1",
        "1 0",
        "0 2",
        "2 0"
    };

    void recieve_and_make_move(GCPClient& client, int move) {
        auto res = client.read_tag("MOVE");
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
        EXPECT_EQ(std::get<1>(res),moves[move-1]);
        res = client.send_move(moves[move]);
        EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    }
};

TEST_F(PlayTest, TwoPlayersWhiteWin) {
    // Connect clients
    EXPECT_TRUE(server.is_running());
    EXPECT_TRUE(client1.connect("127.0.0.1",server.get_port()));
    EXPECT_TRUE(client2.connect("127.0.0.1",server.get_port()));


    // Send Auth for client1
    EXPECT_TRUE(client1.connected());
    client1.send_auth("game","player1");
    auto res = client1.read_tag("AUTH");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"VALID");

    // Send Auth for client2
    EXPECT_TRUE(client2.connected());
    client2.send_auth("game","player2");
    res = client2.read_tag("AUTH");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"VALID");

    // Verify player1 is White
    res = client1.read_tag("SIDE");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"W");

    // Verify player2 is Black
    res = client2.read_tag("SIDE");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"B");

    // Send Move (White)
    res = client1.send_move(moves[0]);
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    // Alternating moves
    recieve_and_make_move(client2,1);
    recieve_and_make_move(client1,2);
    recieve_and_make_move(client2,3);
    recieve_and_make_move(client1,4);
    // White win
    res = client1.read_tag("GAMEOVER");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"WIN");
    // Black lose
    res = client2.read_tag("GAMEOVER");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"LOSE");

    // Disconnect
    server.stop();
    EXPECT_FALSE(server.is_running());
    client1.disconnect();
    EXPECT_FALSE(client1.connected());
    client2.disconnect();
    EXPECT_FALSE(client2.connected());
}