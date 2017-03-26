// GCPServerSocket - Game Client Protocol Socket Library for Server-side
#pragma once

#include <atomic>
#include <memory>
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

const int MAX_MESSAGE = 2048;

class GCPServerSocket {
private:
    int _sockfd;
    std::atomic_bool _connected;

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
        Disconnect
    };
    ServerState server_verify_auth();

public:
    GCPServerSocket(int sockfd);
    ~GCPServerSocket();

    bool connected() { return _connected; }
    void disconnect();

    void start();

}; //end class GCPServerSocket

} // end namespace Morphling::Networking
