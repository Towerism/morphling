#include <networking/gcp_server.h>

using namespace Morphling::Networking;
using namespace Morphling::ServerState;

GCPServer::GCPServer(Gamelogic::Game_engine* engine):
    running(false),
    port(55555),
    sockfd(-1),
    serverstate(new Server_state(engine)),
    connection_thread()
{ }

GCPServer::~GCPServer()
{
    // if the connection_handler is still running, stop it.
    if (running) {
        stop();
    }
}

bool GCPServer::start(int port_no)
{
    // check if the connection_handler is running already
    if (running) {
        return false;
    }
    // set the port to port_no
    port = port_no;

    struct addrinfo hints;
    struct addrinfo* result;
    struct sockaddr_storage sock_addr;
    socklen_t s_size;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        freeaddrinfo(result); // all done with this structure
        return false;
    }
    if ((sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
        perror("server: socket");
        freeaddrinfo(result); // all done with this structure
        return false;
    }
    if (::bind(sockfd, result->ai_addr, result->ai_addrlen) == -1) {
        perror("server: bind");
        freeaddrinfo(result); // all done with this structure
        return false;
    }
    // get the port number if we asked the OS for one
    if (port == 0) {
        s_size = sizeof(sock_addr);
        if ((rv = getsockname(sockfd,(struct sockaddr *)&sock_addr,&s_size)) == -1) {
            perror("server: getsockname");
            freeaddrinfo(result); // all done with this structure
            return false;
        }
        port = ntohs(((struct sockaddr_in*)&sock_addr)->sin_port);
    }
    freeaddrinfo(result); // all done with this structure

    if (listen(sockfd, MAX_BACKLOG) == -1) {
        perror("listen");
        return false;
    }

    // set the server to a running state
    running = true;

    // run the socket accept in a thread
    connection_thread = std::thread(&GCPServer::connection_handler,this);

    return true;
}

bool GCPServer::stop() {
    // set the stop flag for the connection_handler
    running = false;
    // close the accepting socket
    close(sockfd);
    // join the connection_handler thread
    connection_thread.join();
    // join the rest of the threads that are left
    client_wait();

    return !running;
}

void GCPServer::connection_handler()
{
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int newfd;

    struct timeval r_timeout;
    r_timeout.tv_sec = 1;
    r_timeout.tv_usec = 0;

    fd_set read_fd;

    while (running) {  // main accept() loop

        FD_ZERO(&read_fd);
        FD_SET(sockfd, &read_fd);

        sin_size = sizeof(their_addr);
        int rv = select(sockfd+1, &read_fd, nullptr, nullptr, &r_timeout);
        if (rv > 0) {
            newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
            if (newfd == -1) {
                // only print an error if the server is still running
                if (running) {
                    perror("accept");
                }
                continue;
            }
            { // lock access to the client list during addition
                std::unique_lock<std::mutex> lock(client_list_mutex);
                socket_list.push_back(new GCPServerSocket(serverstate,newfd));
                client_list.push_back(std::thread(&GCPServer::client_handler,this,socket_list.back()));
            }
        } else if (rv == 0) {
            // timeout occured
        } else {
            // error occured
            running = false;
        }
    }
}

void GCPServer::client_handler(GCPServerSocket* gcp) {
    gcp->start();
}

void GCPServer::client_wait() {
    std::unique_lock<std::mutex> lock(client_list_mutex);
    // send disconnect to all games
    serverstate->disconnect_all_games();
    for (auto& s: socket_list) {
        s->disconnect();
    }
    // rejoin all serving threads
    for (auto& t: client_list) {
        t.join();
    }
}
