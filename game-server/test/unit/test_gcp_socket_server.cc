#include <gtest/gtest.h>
#include <networking/gcp_server.h>

using namespace Morphling::Networking;

class GCPServerTest : public ::testing::Test {
protected:
    GCPServerTest() {
        EXPECT_EQ(server.start(0),true);
    }

    GCPServer server;
};

TEST_F(GCPServerTest, CanStartAndStop) {
    EXPECT_EQ(server.is_running(),true);
    server.stop();
    EXPECT_EQ(server.is_running(),false);
}

TEST_F(GCPServerTest, AlreadyRunning) {
    EXPECT_EQ(server.is_running(),true);
    EXPECT_EQ(server.start(0),false);
    EXPECT_EQ(server.is_running(),true);
}
