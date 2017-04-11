// GCPServerSocket - Game Client Protocol Socket Library for Server-side
#pragma once

#include <networking/gcp_socket.h>
#include <serverstate/server_state.h>

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <iostream>
#include <string>
#include <tuple>

namespace Morphling::Networking {

class GCPServerSocket : public GCPSocket {
public:
    GCPServerSocket(Morphling::ServerState::Server_state* serverstate, int sockfd);
    ~GCPServerSocket();

    void disconnect() override;
    void start();
private:
    ServerState::Server_state* serverstate;
    ServerState::Server_state::game_instance_t game;
    ServerState::Player* player;
    
    // Direct Socket Functions
    void send_close();

    // State functions
    enum ServerState {
        VerifyAuth,
        WaitForOtherConnect,
        SendWB,
        WaitForMove,
        WaitForOtherMove,
        VerifyMove,
        SendMove,
        InvalidAuthGame,
        InvalidAuthName,
        Disconnect
    };
    ServerState state;
    std::mutex m_player;
    std::condition_variable cv_player;
    ServerState server_verify_auth();
    ServerState server_wait_for_other();
    ServerState server_send_side();
    ServerState server_wait_for_move();
    ServerState server_wait_for_other_move();
    ServerState server_invalid_auth_game();
    ServerState server_invalid_auth_name();

    void wait_for_player(Morphling::ServerState::Player& player);
    bool check_server_status();

}; //end class GCPServerSocket

} // end namespace Morphling::Networking
