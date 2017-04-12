#include <networking/gcp_client.h>

using namespace Morphling::Networking;

GCPClient::GCPClient() : GCPSocket() {}

GCPClient::~GCPClient() {}

// ======================================================================
// Private Functions
// ======================================================================

// ======================================================================
// Public Functions
// ======================================================================

GCPSocket::RET GCPClient::send_auth(std::string gameid, std::string name) {
    // make sure we are still connected
    if (_connected) {
        return swrite("GAME:"+gameid+"~NAME:"+name+"\n");
    }

    return std::make_tuple(Error,"Not Connected");
}

GCPSocket::RET GCPClient::send_move(std::string move) {
    // make sure we are still connected
    if (_connected) {
        return swrite("MOVE:"+move+"\n");
    }

    return std::make_tuple(Error,"Not Connected");
}