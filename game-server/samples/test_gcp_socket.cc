#include <networking/gcp_socket.h>

#include <iostream>
#include <tuple>

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

    // Get the AUTH variables before connecting
    std::string game, name;
    std::cout << "Authentication\nGAME:";
    std::cin >> game;
    std::cout << "NAME:";
    std::cin >> name;

    std::cout << "Connecting..." << std::endl;

    if (!gcp.connect("127.0.0.1",port)) {
        std::cout << "GCP Socket Client failed to start, exiting.." 
            << std::endl;
        return 1;
    }

    std::cout << "GCP Socket Client started: " << std::endl;

    // Send the AUTH
    auto res = gcp.send_auth(game,name);
    if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        std::cout << "> Error Write: " << std::get<1>(res) << std::endl;
    }
    res = gcp.sread_wait();
    if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
    }
    std::cout << "> " << std::get<1>(res) << std::endl;

    std::string input;
    while (input != "EXIT") {
        // process user input
        std::cout << "< ";
        std::cin >> input;
        auto res = gcp.swrite(input);
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Write: " << std::get<1>(res) << std::endl;
            break;
        }
        // wait for a response
        res = gcp.sread_wait();
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
            break;
        }
        // Check for BYE
        if (std::get<1>(res) == "BYE") input = "EXIT";
        std::cout << "> " << std::get<1>(res) << std::endl;
    }

    // Disconnect and shutdown
    std::cout << "GCP Socket Client shutting down.." << std::endl;

    gcp.disconnect();

    return 0;
}
