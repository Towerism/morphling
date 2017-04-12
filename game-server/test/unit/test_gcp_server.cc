#include <gtest/gtest.h>
#include <networking/gcp_server.h>
#include <networking/gcp_client.h>
#include <gamelogic/games/tictactoe/tictactoe_engine.h>

using namespace Morphling::Networking;
using namespace Morphling::Gamelogic;

class GCPServerTest : public ::testing::Test {
protected:
    GCPServerTest(): server(new Tictactoe::Tictactoe_engine()) {
        EXPECT_TRUE(server.start(0));
        EXPECT_TRUE(server.is_running());
    }

    GCPServer server;
    GCPClient clientDummy;
    GCPClient client1;
    GCPClient client2;
};

TEST_F(GCPServerTest, CanStartAndStop) {
    server.stop();
    EXPECT_EQ(server.is_running(),false);
}

TEST_F(GCPServerTest, AlreadyRunning) {
    EXPECT_EQ(server.start(0),false);
    EXPECT_EQ(server.is_running(),true);
}

TEST_F(GCPServerTest, ClientDisconnectFirst) {
    EXPECT_TRUE(clientDummy.connect("127.0.0.1",server.get_port()));
    EXPECT_TRUE(clientDummy.connected());
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
}

TEST_F(GCPServerTest, ServerDisconnectFirst) {
    EXPECT_TRUE(clientDummy.connect("127.0.0.1",server.get_port()));
    EXPECT_TRUE(clientDummy.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
}

TEST_F(GCPServerTest, OneAuthenticatedPlayer) {
    EXPECT_TRUE(server.is_running());
    EXPECT_TRUE(clientDummy.connect("127.0.0.1",server.get_port()));

    // Send Auth
    EXPECT_TRUE(clientDummy.connected());
    clientDummy.send_auth("game","player1");
    auto res = clientDummy.read_tag("AUTH");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"VALID");

    // Disconnect
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
}

TEST_F(GCPServerTest, InvalidGameAuthentication) {
    EXPECT_TRUE(server.is_running());
    EXPECT_TRUE(clientDummy.connect("127.0.0.1",server.get_port()));

    // Send Auth
    EXPECT_TRUE(clientDummy.connected());
    clientDummy.send_auth("badgame","player1");
    auto res = clientDummy.read_tag("AUTH");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"INVALIDGAME");

    // Disconnect
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
}

TEST_F(GCPServerTest, InvalidNameAuthentication) {
    EXPECT_TRUE(server.is_running());
    EXPECT_TRUE(clientDummy.connect("127.0.0.1",server.get_port()));

    // Send Auth
    EXPECT_TRUE(clientDummy.connected());
    clientDummy.send_auth("game","player3");
    auto res = clientDummy.read_tag("AUTH");
    EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
    EXPECT_EQ(std::get<1>(res),"INVALIDNAME");

    // Disconnect
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
}

// TEST_F(GCPServerTest, TwoAuthenticatedPlayers) {
//     // Connect clients
//     EXPECT_TRUE(server.is_running());
//     EXPECT_TRUE(client1.connect("127.0.0.1",server.get_port()));
//     EXPECT_TRUE(client2.connect("127.0.0.1",server.get_port()));

//     // Send Auth for client1
//     EXPECT_TRUE(client1.connected());
//     client1.send_auth("game","player1");
//     auto res = client1.read_tag("AUTH");
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
//     EXPECT_EQ(std::get<1>(res),"VALID");

//     // Send Auth for client2
//     EXPECT_TRUE(client2.connected());
//     client2.send_auth("game","player2");
//     res = client2.read_tag("AUTH");
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
//     EXPECT_EQ(std::get<1>(res),"VALID");

//     // Disconnect
//     server.stop();
//     EXPECT_FALSE(server.is_running());
//     client1.disconnect();
//     EXPECT_FALSE(client1.connected());
//     client2.disconnect();
//     EXPECT_FALSE(client2.connected());
// }

// TEST_F(GCPServerTest, TwoPlayersWBStateCheck) {
//     // Connect clients
//     EXPECT_TRUE(server.is_running());
//     EXPECT_TRUE(client1.connect("127.0.0.1",server.get_port()));
//     EXPECT_TRUE(client2.connect("127.0.0.1",server.get_port()));


//     // Send Auth for client1
//     EXPECT_TRUE(client1.connected());
//     client1.send_auth("game","player1");
//     auto res = client1.read_tag("AUTH");
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
//     EXPECT_EQ(std::get<1>(res),"VALID");

//     // Send Auth for client2
//     EXPECT_TRUE(client2.connected());
//     client2.send_auth("game","player2");
//     res = client2.read_tag("AUTH");
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
//     EXPECT_EQ(std::get<1>(res),"VALID");

//     // Verify player1 is White
//     res = client1.read_tag("SIDE");
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
//     EXPECT_EQ(std::get<1>(res),"W");

//     // Verify player2 is Black
//     res = client2.read_tag("SIDE");
//     EXPECT_EQ(std::get<0>(res),GCPSocket::SocketReturn::Ok);
//     EXPECT_EQ(std::get<1>(res),"B");

//     // Disconnect
//     server.stop();
//     EXPECT_FALSE(server.is_running());
//     client1.disconnect();
//     EXPECT_FALSE(client1.connected());
//     client2.disconnect();
//     EXPECT_FALSE(client2.connected());
// }