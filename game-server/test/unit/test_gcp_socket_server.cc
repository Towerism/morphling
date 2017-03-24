#include <gtest/gtest.h>
#include <networking/gcp_socket_server.h>

using namespace Morphling::Networking;

class GCPSServerTest : public ::testing::Test {
protected:
    GCPSServerTest() {
    }

    GCPSServer server;
    int port_no = 55555;

    // alternate ports so it doesn't collide with multiple bind/listen
    int port() { return port_no++; }
};

TEST_F(GCPSServerTest, CanStartAndStop) {
    EXPECT_EQ(server.start(port()),true);
    EXPECT_EQ(server.is_running(),true);
    server.stop();
    EXPECT_EQ(server.is_running(),false);
}

TEST_F(GCPSServerTest, AlreadyRunning) {
    EXPECT_EQ(server.start(port()),true);
    EXPECT_EQ(server.is_running(),true);
    EXPECT_EQ(server.start(),false);
}
