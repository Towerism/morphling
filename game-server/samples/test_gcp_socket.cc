#include <networking/gcp_client.h>

#include <iostream>
#include <tuple>

using namespace Morphling::Networking;

int shutdown(GCPClient& gcp, int ret) {
    // Disconnect and shutdown
    std::cout << "GCP Socket Client shutting down.." << std::endl;

    gcp.disconnect();
    return ret;
}

int repl(GCPClient& gcp) {
    std::string input;
    std::cin.clear();
    std::getline(std::cin,input);
    while (!gcp.is_gameover()) {
        // process user input
        std::cout << "< ";
        std::getline(std::cin,input);
        auto res = gcp.send_move(input);
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Write: " << std::get<1>(res) << std::endl;
            return 1;
        }
        // wait for a response
        res = gcp.recv_move();
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
            return 1;
        }
        // print out other move by other player
        std::cout << "> " << std::get<1>(res) << std::endl;
    }
    return 0;
}

int main(int argc, char** argv) {

    GCPClient gcp;

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
        return shutdown(gcp,1);
    }
    res = gcp.recv_auth();
    if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        return shutdown(gcp,1);
    }
    std::cout << "> " << std::get<1>(res) << std::endl;

    // Wait for which side
    res = gcp.recv_side();
    if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        return shutdown(gcp,1);
    }
    std::cout << "> " << std::get<1>(res) << std::endl;

    // If we are Black wait
    if (std::get<1>(res) == "B") {
        // wait for a response
        res = gcp.recv_move();
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
            return shutdown(gcp,1);

        }
        // print out other move by other player
        std::cout << "> " << std::get<1>(res) << std::endl;
    }
    
    return shutdown(gcp,repl(gcp));
}
