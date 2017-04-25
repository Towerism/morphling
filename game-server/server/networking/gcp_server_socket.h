// GCPServerSocket - Game Client Protocol Socket Library for Server-side
#pragma once

#include <networking/gcp_socket.h>
#include <serverstate/server_state.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <iostream>
#include <string>
#include <tuple>

#if 0
#define STATELOG(t) std::cerr << "State: " << t << std::endl;
#else
#define STATELOG(t) ;
#endif

namespace Morphling::Networking {

class GCPServerSocket : public GCPSocket {
public:
    GCPServerSocket(Morphling::ServerState::Server_state* serverstate, int sockfd);
    ~GCPServerSocket();

    void disconnect() override;
    void baddisconnect();
    void start();
private:
    ServerState::Server_state* serverstate;
    ServerState::Server_state::game_instance_t game;
    ServerState::Player* player;
    
    // State functions
    enum ServerState {
        VerifyAuth,
        WaitForOtherConnect,
        SendWB,
        WaitForMove,
        WaitForOtherMove,
        VerifyMove,
        SendMove,
        GameOver,
        InvalidAuthGame,
        InvalidAuthName,
        InvalidAuthGameInSession,
        InvalidMove,
        BadDisconnect,
        GoodDisconnect
    };
    ServerState state;
    std::mutex m_player;
    std::condition_variable cv_player;

    std::mutex m_delay;
    std::condition_variable cv_delay;

    // Normal States
    ServerState server_verify_auth();
    ServerState server_send_side();
    ServerState server_wait_for_move();
    ServerState server_wait_for_other_move();
    ServerState server_verify_move();
    ServerState server_send_move();
    ServerState server_send_gameover();

    // Invalid States
    ServerState server_invalid_auth_game();
    ServerState server_invalid_auth_name();
    ServerState server_invalid_auth_game_in_session();
    ServerState server_invalid_move();

    ServerState barrier(ServerState next_state);
    void wait_for_player(Morphling::ServerState::Player& player);
    bool check_server_status();

}; //end class GCPServerSocket

} // end namespace Morphling::Networking
