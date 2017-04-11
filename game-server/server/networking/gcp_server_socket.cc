#include <networking/gcp_server_socket.h>

using namespace Morphling::Networking;
using namespace Morphling::ServerState;

GCPServerSocket::GCPServerSocket(Server_state* ss, int sockfd): 
    GCPSocket(sockfd),
    serverstate(ss)
{ }


GCPServerSocket::~GCPServerSocket() {
    disconnect();
}

// ======================================================================
// Socket Direct Access Functions
// ======================================================================

// Send a proper "BYE" to before closing the connection
void GCPServerSocket::send_close() {
    // Send the disconnect code: BYE
    auto ret = swrite("BYE\n");
    // doesn't truly matter if the response was received, the client may
    // disconnect automatically.
    if (std::get<0>(ret) != Ok)
        return;
}

// ======================================================================
// State Functions
// ======================================================================

// Read the first GCP message, which should be of the form: AUTH:xxxxx
// Process the Authentication to connect the client to the proper game
// server state.
GCPServerSocket::ServerState GCPServerSocket::server_verify_auth() {
    auto auth = read_tags("GAME","NAME");
    // if the read_tag failed, disconnect
    if (std::get<0>(auth) != Ok) {
        //std::cout << std::get<1>(auth) << std::endl;
        return Disconnect;
    }
    
    Server_state::game_instance_t check = serverstate->get_game(std::get<1>(auth),std::get<2>(auth));
    if (check == nullptr) {
        // the game was invalid
        return InvalidAuthGame;
    }
    game = check;
    auto check_player = game->check_player_name(std::get<2>(auth));
    if (std::get<0>(check_player) == false) {
        // the player was not valid (should not happed as previous
        // 'check' does the same operation)
        return InvalidAuthName;
    }
    player = std::get<1>(check_player);
    player->ready = true;

    // send valid authentication
    auto valid_auth = swrite("VALIDAUTH\n");
    if (std::get<0>(valid_auth) != Ok) {
        return Disconnect;
    }
    
    return WaitForOtherConnect;
}

// Wait for the next player to join. Or if this is the last player to 
// to join, continue.
GCPServerSocket::ServerState GCPServerSocket::server_wait_for_other() {
    
    // check if the other player has already connected
    if (player->side == White) {
        if (game->player2.ready) {
            //std::cout << "both ready 1" << std::endl;
            game->move_cv.notify_all();
            std::unique_lock<std::mutex> lkpost(game->move_post_mutex);
            game->move_post_cv.wait(lkpost, [&]{return not game->player1.ready and not game->player2.ready;});
        } else {
            // otherwise wait for the other player to be ready
            //std::cout << "waiting for B" << std::endl;
            wait_for_player(game->player2);
            game->player2.ready = false;
            game->player1.ready = false;
            game->move_post_cv.notify_all();
        }
    } else {
        if (game->player1.ready) {
            //std::cout << "both ready 2" << std::endl;
            game->move_cv.notify_all();
            std::unique_lock<std::mutex> lkpost(game->move_post_mutex);
            game->move_post_cv.wait(lkpost, [&]{return not game->player1.ready and not game->player2.ready;});
        } else {
            // otherwise wait for the other player to be ready
            //std::cout << "waiting for W" << std::endl;
            wait_for_player(game->player1);
            game->player2.ready = false;
            game->player1.ready = false;
            game->move_post_cv.notify_all();
        }
    }
    
    return SendWB;
}

// Send this player which side they are on based on the tournament data.
// Then wait for either this player, or the other player to make a move
// (white moves first).
GCPServerSocket::ServerState GCPServerSocket::server_send_side() {
    // send the side 
    std::string side;
    if (player->side == White) side = "W"; else side = "B";
    auto ret = swrite("SIDE:"+side);

    // check the send status
    if (std::get<0>(ret) != Ok) {
        // TODO(devincarr): check the reason for the send failure (if needed).
        return Disconnect;
    }

    // transition to the wait step for the move if PlayerSide::White
    if (player->side == White) {
        return WaitForMove;
    } else {
        // otherwise wait for the other player to make a move
        return WaitForOtherMove;
    }
}

GCPServerSocket::ServerState GCPServerSocket::server_wait_for_move() {
    // wait for the player to make a move
    auto move = sread_wait();
    // TODO(devincarr): What happens when a Timeout occurs, is it different from Disconnect?
    if (std::get<0>(move) == Timeout) {
        return Disconnect;
    }
    if (std::get<0>(move) != Ok) {
        return Disconnect;
    }

    // Everything is okay, verify the move
    return VerifyMove;
}

GCPServerSocket::ServerState GCPServerSocket::server_wait_for_other_move() {
    if (game->player_turn == White) {
        wait_for_player(game->player1);
    } else {
        wait_for_player(game->player2);
    }

    return SendMove;
}

GCPServerSocket::ServerState GCPServerSocket::server_invalid_auth_game() {
    auto ret = swrite("INVALIDAUTH: GAME\n");
    // doesn't truly matter if the response was received, the client may
    // disconnect automatically.
    return Disconnect;
}

GCPServerSocket::ServerState GCPServerSocket::server_invalid_auth_name() {
    auto ret = swrite("INVALIDAUTH: NAME\n");
    // doesn't truly matter if the response was received, the client may
    // disconnect automatically.
    return Disconnect;
}

void GCPServerSocket::wait_for_player(Player& other_player) {
    std::unique_lock<std::mutex> lk(game->move_mutex);
    game->move_cv.wait(lk, [&] {
        return other_player.ready or not _connected or not game->running;
    });
}

bool GCPServerSocket::check_server_status() {
    if (game == nullptr) {
        return state != Disconnect and _connected;
    } else {
        return state != Disconnect and _connected and game->running;
    }
}

// ======================================================================
// Public Functions
// ======================================================================

void GCPServerSocket::disconnect() {
    state = Disconnect;
    GCPSocket::disconnect();
}

// Entry point for the start of the server state response state machine
void GCPServerSocket::start() {
    state = VerifyAuth;
    // enter the state machine
    while (check_server_status()) {
        // process and transistion to the next state
        switch (state) {
            case VerifyAuth: {
                //std::cout << "VerifyAuth" << std::endl;
                state = server_verify_auth();
                break;
            }
            case WaitForOtherConnect: {
                //std::cout << "WaitForOtherConnect" << std::endl;
                state = server_wait_for_other();
                break;
            }
            case SendWB: {
                //std::cout << "player: " << player->side << ", ready to send WB" << std::endl;
                state = server_send_side();
                break;
            }
            case WaitForMove: {
                // TODO(devincarr): fall through for now
                state = server_wait_for_move();
                break;
            }
            case WaitForOtherMove: {
                // TODO(devincarr): fall through for now
                state = server_wait_for_other_move();
                break;
            }
            case VerifyMove: {
                // TODO(devincarr): fall through for now
                state = Disconnect;
                break;
            }
            case SendMove: {
                // TODO(devincarr): fall through for now
                state = Disconnect;
                break;
            }
            case InvalidAuthGame: {
                state = server_invalid_auth_game();
                break;
            }
            case InvalidAuthName: {
                state = server_invalid_auth_name();
                break;
            }
            default:
            case Disconnect: {
                // A disconnect was called
                // TODO(devincarr): Verify if it was a hard disconnect or error in response
                // Assume safe disconnect and send message
                if (_connected) send_close();
                disconnect();
                return;
            }
        }
    }
    disconnect();
}

