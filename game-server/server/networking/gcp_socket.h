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
    int _sockfd;

protected:
    std::atomic_bool _connected;
    bool dns(std::string hostname, int port, struct sockaddr_in* server);

    size_t _tries;
    time_t _sock_wait;

public:
    // Max time to wait before a timeout (seconds)
    static const time_t MAX_TIMEOUT = 20;
    static const size_t NORMAL_TIMEOUT = 2;
    // Maximum amount of times to wait
    static const size_t MAX_TRIES = 60;
    static const size_t NORMAL_TRIES = 20;

    enum SocketReturn {
        Ok,
        Error,
        Timeout
    };
    typedef std::tuple<GCPSocket::SocketReturn,std::string> RET;
    typedef std::tuple<GCPSocket::SocketReturn,std::string,std::string> RTAGS;

    GCPSocket(int tries = NORMAL_TRIES, int seconds = NORMAL_TIMEOUT);
    GCPSocket(int _sockfd, int tries = NORMAL_TRIES, int seconds = NORMAL_TIMEOUT);
    ~GCPSocket();

    bool connected() { return _connected; }
    virtual bool connect(std::string hostname, int port);
    virtual void disconnect();

    // Socket Direct Access Functions
    RET response;
    RET sread();
    RET sread_wait();
    RET swrite(std::string msg);
    RET read_tag(std::string input, std::string tag);
    RTAGS read_tags(std::string input, std::string tag1, std::string tag2);

}; //end class GCPSocket

} // end namespace Morphling::Networking
