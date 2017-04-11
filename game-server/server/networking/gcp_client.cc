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
        auto res = swrite("GAME:"+gameid+"~NAME:"+name+"\n");
        // make sure the swrite completed successfully
        if (std::get<0>(res) != Ok) {
            return res;
        }
        return std::make_tuple(Ok,"");
    }

    return std::make_tuple(Error,"Not Connected");
}


