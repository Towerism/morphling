#include <gtest/gtest.h>
#include <networking/gcp_server.h>
#include <networking/gcp_socket.h>

using namespace Morphling::Networking;

class GCPServerTest : public ::testing::Test {
protected:
    GCPServerTest() {
        EXPECT_TRUE(server.start(55555));
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