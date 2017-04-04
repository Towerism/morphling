#include <gtest/gtest.h>
#include <networking/gcp_server.h>
#include <networking/gcp_socket.h>
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
    GCPSocket clientDummy;
    GCPSocket client1;
    GCPSocket client2;
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
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
}

TEST_F(GCPServerTest, ServerDisconnectFirst) {
    EXPECT_TRUE(clientDummy.connect("127.0.0.1",server.get_port()));
    server.stop();
    EXPECT_FALSE(server.is_running());
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
}

TEST_F(GCPServerTest, OneAuthenticatedPlayer) {
    EXPECT_TRUE(clientDummy.connect("127.0.0.1",server.get_port()));
    EXPECT_TRUE(server.is_running());

    // Send Auth
    EXPECT_TRUE(clientDummy.connected());
    clientDummy.send_auth("game","player1");
    auto res = clientDummy.sread_wait();
    EXPECT_TRUE(std::get<0>(res));
    EXPECT_EQ(std::get<1>(res),"VALIDAUTH\n");

    // Disconnect
    clientDummy.disconnect();
    EXPECT_FALSE(clientDummy.connected());
    server.stop();
    EXPECT_FALSE(server.is_running());
}