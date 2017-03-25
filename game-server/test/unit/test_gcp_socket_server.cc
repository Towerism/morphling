#include <gtest/gtest.h>
#include <networking/gcp_socket_server.h>
#include <networking/gcp_socket.h>

using namespace Morphling::Networking;

class GCPSServerTest : public ::testing::Test {
protected:
    GCPSServerTest() {
        EXPECT_EQ(server.start(0),true);
    }

    GCPSServer server;
    GCPSocket sock;
};

TEST_F(GCPSServerTest, CanStartAndStop) {
    EXPECT_EQ(server.is_running(),true);
    server.stop();
    EXPECT_EQ(server.is_running(),false);
}

TEST_F(GCPSServerTest, AlreadyRunning) {
    EXPECT_EQ(server.is_running(),true);
    EXPECT_EQ(server.start(0),false);
    EXPECT_EQ(server.is_running(),true);
}
