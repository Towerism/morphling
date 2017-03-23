#include <networking/gcp_socket_server.h>

using namespace Morphling::Networking;

GCPSServer::GCPSServer(): 
    running(false),
    port("55555"),
    sockfd(-1),
    connection_thread()
{ }

GCPSServer::~GCPSServer()
{
    // if the connection_handler is still running, stop it.
    if (running) {
        stop();
    }
}

bool GCPSServer::start(const char* port_no = "55555")
{
    // check if the connection_handler is running already
    if (running) {
        return false;
    }
    // set the port to port_no
    port = port_no;

    struct addrinfo hints;
    struct addrinfo* result;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port, &hints, &result)) != 0) {
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
    freeaddrinfo(result); // all done with this structure

    if (listen(sockfd, MAX_BACKLOG) == -1) {
        perror("listen");
        return false;
    }

    // set the server to a running state
    running = true;

    // run the socket accept in a thread
    connection_thread = std::move(std::thread(&GCPSServer::connection_handler,this));

    return true;
}

bool GCPSServer::stop() {
    // set the stop flag for the connection_handler
    running = false;
    // close the accepting socket
    close(sockfd);
    // join the connection_handler thread
    connection_thread.join();

    return !running;
}

void GCPSServer::connection_handler()
{
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int newfd;

    printf("server: waiting for connections...\n");
    while (running) {  // main accept() loop
        sin_size = sizeof(their_addr);
        newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }
        //pthread_t new_thread;
        printf("accepted new client: %d\n", newfd);
        close(newfd);
        /*
        NetworkRequestChannel* newnrc = new NetworkRequestChannel(newfd);
        pthread_create(&new_thread,NULL,connection_handler,newnrc);
        */
    }
}
