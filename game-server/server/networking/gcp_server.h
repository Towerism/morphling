// GCPServer - Game Client Protocol Server
// Front End to handle distributing incoming GCP client connections

#pragma once

#include <serverstate/server_state.h>
#include <gamelogic/controller/game_engine.h>
#include <networking/gcp_server_socket.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

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

class GCPServer {
private:
    std::atomic_bool running;

    int port;
    int sockfd;

    ServerState::Server_state* serverstate;

    std::thread connection_thread;
    std::mutex client_list_mutex;
    std::vector<GCPServerSocket*> socket_list;
    std::vector<std::thread> client_list;

    // Functions
    void connection_handler();
    void client_handler(GCPServerSocket* gcp);
    void client_wait();

    bool send(std::string msg);
public:
    GCPServer(Gamelogic::Game_engine* engine, std::string firebase_proj);
    ~GCPServer();

    bool start(int port_no = 55555);
    bool stop();
    bool initialize_firebase();
    bool is_running() { return running; }
    int get_port() const { return port; }

}; // end class GCPServer

} // end namespace Morphling::Networking
