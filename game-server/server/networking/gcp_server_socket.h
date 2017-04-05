// GCPServerSocket - Game Client Protocol Socket Library for Server-side
#pragma once

#include <serverstate/server_state.h>

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <iostream>
#include <string>
#include <tuple>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

namespace Morphling::Networking {

class GCPServerSocket {
public:
    enum SocketReturn {
        Ok,
        Error,
        Timeout
    };
    typedef std::tuple<GCPServerSocket::SocketReturn,std::string> RET;
    typedef std::tuple<GCPServerSocket::SocketReturn,std::string,std::string> RTAGS;
    
    GCPServerSocket(Morphling::ServerState::Server_state* serverstate, int sockfd);
    ~GCPServerSocket();

    bool connected() { return _connected; }
    void disconnect();

    void start();
private:
    const int MAX_MESSAGE = 2048;
    // Max time to wait before a timeout (seconds)
    const time_t MAX_TIMEOUT = 20;
    
    int _sockfd;
    std::atomic_bool _connected;

    ServerState::Server_state* serverstate;
    ServerState::Server_state::game_instance_t game;
    ServerState::Player* player;

    // Max number of timeouts for waiting on a read
    int max_tries;
    
    // Direct Socket Functions
    RET sread();
    RET sread_wait(time_t seconds = 2);
    RET swrite(std::string msg);
    RET read_tag(std::string tag);
    RTAGS read_tags(std::string tag1, std::string tag2);
    void send_close();

    // State functions
    enum ServerState {
        VerifyAuth,
        WaitForOtherConnect,
        SendWB,
        WaitForMove,
        WaitForOtherMove,
        VerifyMove,
        SendMove,
        InvalidAuthGame,
        InvalidAuthName,
        Disconnect
    };
    std::mutex m_player;
    std::condition_variable cv_player;
    ServerState server_verify_auth();
    ServerState server_wait_for_other();
    ServerState server_send_side();
    ServerState server_wait_for_move();
    ServerState server_wait_for_other_move();
    ServerState server_invalid_auth_game();
    ServerState server_invalid_auth_name();

    bool wait_for_player(Morphling::ServerState::Player& player);
    bool check_server_status(ServerState state);

}; //end class GCPServerSocket

} // end namespace Morphling::Networking
