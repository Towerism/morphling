// GCPSServer - Game Client Protocol Socket Server
#pragma once

#include <atomic>
#include <memory>
#include <iostream>
#include <string>
#include <thread>

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

const int MAX_BACKLOG = 64;
const int MAX_MESSAGE = 2048;

class GCPSServer {
private:
    std::atomic_bool running;

    int port;
    int sockfd;
    std::thread connection_thread;

    void connection_handler();
    // return the port number as a const char* for getaddrinfo
    const char* port_str() { return std::to_string(port).c_str(); }
public:
    GCPSServer();
    ~GCPSServer();

    bool start(int port_no = 55555);
    bool stop();

    bool is_running() { return running; }
}; // end class GCPSServer

} // end namespace Morphling::Networking
