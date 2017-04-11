// GCPSocket - Game Client Protocol Socket Library
#pragma once

#include <atomic>
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

// Max message size for transmission
#define MAX_MESSAGE 512

namespace Morphling::Networking {

class GCPSocket {
private:
    // Max time to wait before a timeout (seconds)
    const time_t MAX_TIMEOUT = 10;
    // Maximum amount of times to wait
    const size_t MAX_TRIES = 10;
    int _sockfd;

protected:
    std::atomic_bool _connected;
    bool dns(std::string hostname, int port, struct sockaddr_in* server);

public:

    enum SocketReturn {
        Ok,
        Error,
        Timeout
    };
    typedef std::tuple<GCPSocket::SocketReturn,std::string> RET;
    typedef std::tuple<GCPSocket::SocketReturn,std::string,std::string> RTAGS;

    GCPSocket();
    GCPSocket(int _sockfd);
    ~GCPSocket();

    bool connected() { return _connected; }
    virtual bool connect(std::string hostname, int port);
    virtual void disconnect();

    // Socket Direct Access Functions
    RET sread();
    RET sread_wait(time_t seconds = 2, size_t tries = 10);
    RET swrite(std::string msg);
    RET read_tag(std::string tag);
    RTAGS read_tags(std::string tag1, std::string tag2);

}; //end class GCPSocket

} // end namespace Morphling::Networking
