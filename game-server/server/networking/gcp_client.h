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
    bool gameover;
    bool winner;

    bool check_gameover(std::string input);
public:

    GCPClient();
    ~GCPClient();

    bool is_gameover() { return gameover; }
    bool is_winner() { return winner; }

    GCPSocket::RET send_auth(std::string gameid, std::string name);
    GCPSocket::RET recv_auth();
    GCPSocket::RET recv_side();
    GCPSocket::RET send_move(std::string move);
    GCPSocket::RET recv_move();

}; //end class GCPClient

} // end namespace Morphling::Networking
