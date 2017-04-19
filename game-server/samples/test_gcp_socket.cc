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

void repl(GCPClient& gcp) {
    std::string input;
    while (input != "EXIT") {
        // process user input
        std::cout << "< ";
        std::getline(std::cin,input);
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
        // res = gcp.recv_move();
        // if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        //     std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        //     return shutdown(gcp,1);
        // }
        // std::cout << "> " << std::get<1>(res) << std::endl;
        // // Send a move
        // std::cout << "< MOVE:0 1" << std::endl;
        // res = gcp.send_move("0 1");
        // if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        //     std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        //     return shutdown(gcp,1);
        // }
        // std::cout << "> " << std::get<1>(res) << std::endl;

        // // Read a move
        // res = gcp.recv_move();
        // if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        //     std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        //     return shutdown(gcp,1);
        // }
        // std::cout << "> " << std::get<1>(res) << std::endl;
        // // Send a move
        // std::cout << "< MOVE:0 2" << std::endl;
        // res = gcp.send_move("0 2");
        // if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        //     std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        //     return shutdown(gcp,1);
        // }
        // std::cout << "> " << std::get<1>(res) << std::endl;

        // // Read a move
        // res = gcp.recv_move();
        // if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        //     std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        //     return shutdown(gcp,1);
        // }
        // std::cout << "> " << std::get<1>(res) << std::endl;
    } else {
        // Send a move
        std::cout << "< MOVE:3 4" << std::endl;
        res = gcp.send_move("3 4");
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
            return shutdown(gcp,1);
        }
        std::cout << "> " << std::get<1>(res) << std::endl;

        // Read a move
        res = gcp.recv_move();
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
            return shutdown(gcp,1);
        }
        std::cout << "> " << std::get<1>(res) << std::endl;
        // Send a move
        std::cout << "< MOVE:4 3" << std::endl;
        res = gcp.send_move("1 0");
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
            return shutdown(gcp,1);
        }
        std::cout << "> " << std::get<1>(res) << std::endl;

        // Read a move
        res = gcp.recv_move();
        if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
            std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
            return shutdown(gcp,1);
        }
        std::cout << "> " << std::get<1>(res) << std::endl;
        // Send a move
        // std::cout << "< MOVE:2 0" << std::endl;
        // res = gcp.send_move("2 0");
        // if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        //     std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        //     return shutdown(gcp,1);
        // }
        // std::cout << "> " << std::get<1>(res) << std::endl;

        // // Read a move
        // res = gcp.recv_move();
        // if (std::get<0>(res) != GCPSocket::SocketReturn::Ok) {
        //     std::cout << "> Error Read: " << std::get<1>(res) << std::endl;
        //     return shutdown(gcp,1);
        // }
        // std::cout << "> " << std::get<1>(res) << std::endl;
    }

    // repl(gcp);

    return shutdown(gcp,0);
}
