// GCPServerSocket - Game Client Protocol Socket Library for Server-side
#pragma once

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

#define RET std::tuple<Morphling::Networking::GCPServerSocket::SocketReturn,std::string>

namespace Morphling::Networking {

enum PlayerSide {
    White,
    Black
};

// TODO(devincarr): Make this a part of the Game state later
struct players_ready {
    bool player1;
    bool player2;
};

class GCPServerSocket {
private:
    const int MAX_MESSAGE = 2048;
    
    int _sockfd;
    std::atomic_bool _connected;
    std::string playerid;
    PlayerSide playerside;

    // Direct Socket Functions
    enum SocketReturn {
        Ok,
        Error,
        Timeout
    };

    RET sread();
    RET swrite(std::string msg);
    RET read_tag(std::string tag);
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
        Disconnect
    };
    std::mutex m_player;
    std::condition_variable cv_player;
    ServerState server_verify_auth();
    ServerState server_wait_for_other(players_ready* rdy);
    ServerState server_send_side();

public:
    GCPServerSocket(int sockfd);
    ~GCPServerSocket();

    bool connected() { return _connected; }
    void disconnect();

    void start();

}; //end class GCPServerSocket

} // end namespace Morphling::Networking
