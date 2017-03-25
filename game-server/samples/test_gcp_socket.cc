#include <networking/gcp_socket.h>

#include <iostream>

using namespace Morphling::Networking;

int main(int argc, char** argv) {

    GCPSocket gcp;

    int port;
    if (argc > 1) {
        port = atoi(argv[1]);
    } else {
        std::cout << "Error: no port provided" << std::endl;
        return 1;
    }

    if (!gcp.connect("127.0.0.1",port)) {
        std::cout << "GCP Socket Client failed to start, exiting.." 
            << std::endl;
        return 1;
    }

    std::cout << "GCP Socket Client started: " << std::endl;

    gcp.send_auth();

    std::cout << "< send_auth\n" << "> " << gcp.sread() << std::endl;

    std::cout << "> " << gcp.sread() << std::endl;

    std::cout << "GCP Socket Client shutting down.." << std::endl;

    gcp.disconnect();

    return 0;
}
