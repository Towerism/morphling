// GCPSocket - Game Client Protocol Socket Library
#pragma once

#include <atomic>
#include <memory>
#include <iostream>
#include <string>

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

const int MAX_MESSAGE = 2048;

class GCPSocket {
private:
    int _sockfd;
    std::atomic_bool _connected;

    bool dns(std::string hostname, int port, struct sockaddr_in* server);
    bool swrite(std::string msg);
public:
    GCPSocket();
    ~GCPSocket();

    bool connected() { return _connected; }

    bool connect(std::string hostname, int port);
    void disconnect();

    //TODO(devincarr): make private once sending/reading functions are fully defined
    std::string sread();
    void send_auth(std::string auth);

}; //end class GCPSocket

} // end namespace Morphling::Networking
