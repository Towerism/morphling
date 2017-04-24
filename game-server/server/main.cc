#include <networking/gcp_server.h>
#include <games/tictactoe/tictactoe_engine.h>

#include <iostream>

using namespace Morphling::Networking;
using namespace Morphling::Gamelogic;

int main(int argc, char** argv) {

    GCPServer server(new Tictactoe::Tictactoe_engine(),"flask-experiment-e7196");

    int port;
    if (argc > 1) {
        port = atoi(argv[1]);
    } else {
        // bind to a random port
        port = 55555;
    }

    if (!server.start(port)) {
        std::cout << "GCP Socket Server failed to start, exiting.." 
            << std::endl;
        return 1;
    }

    std::cout << "GCP Socket Server started on port: " 
        << server.get_port() << std::endl;

    char a;
    std::cin >> a;

    std::cout << "GCP Socket Server shutting down.." << std::endl;
    server.stop();

    return 0;
}
