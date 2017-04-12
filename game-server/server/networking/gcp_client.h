// GCPClient - Game Client Protocol Socket Library
#pragma once

#include <networking/gcp_socket.h>

#include <atomic>
#include <memory>
#include <iostream>
#include <string>
#include <tuple>

namespace Morphling::Networking {

class GCPClient : public GCPSocket {
private:
public:

    GCPClient();
    ~GCPClient();

    GCPSocket::RET send_auth(std::string gameid, std::string name);
    GCPSocket::RET send_move(std::string move);

}; //end class GCPClient

} // end namespace Morphling::Networking
