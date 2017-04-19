#include <networking/gcp_server_socket.h>

using namespace Morphling::Networking;
using namespace Morphling::ServerState;

GCPServerSocket::GCPServerSocket(Server_state* ss, int sockfd): 
    GCPSocket(sockfd),
    serverstate(ss)
{ }


GCPServerSocket::~GCPServerSocket() {
    if (_connected) {
        disconnect();
    }
}

// ======================================================================
// State Functions
// ======================================================================

// Read the first GCP message, which should be of the form: AUTH:xxxxx
// Process the Authentication to connect the client to the proper game
// server state.
GCPServerSocket::ServerState GCPServerSocket::server_verify_auth() {
    response = sread_wait();
    // if the read_tag failed, disconnect
    if (std::get<0>(response) != Ok) {
        //std::cout << std::get<1>(auth) << std::endl;
        return BadDisconnect;
    }
    auto auth = read_tags(std::get<1>(response),"GAME","NAME");
    response = std::make_tuple(std::get<0>(auth),std::get<1>(auth));
    
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
    // player->ready = true;
    // game->move_cv.notify_all();

    // send valid authentication
    response = swrite("AUTH:VALID\n");
    if (std::get<0>(response) != Ok) {
        return BadDisconnect;
    }
    
    return WaitForOtherConnect;
}

// Send this player which side they are on based on the tournament data.
// Then wait for either this player, or the other player to make a move
// (white moves first).
GCPServerSocket::ServerState GCPServerSocket::server_send_side() {
    // send the side 
    std::string side;
    if (player->side == White) side = "W"; else side = "B";
    response = swrite("SIDE:"+side);

    // check the send status
    if (std::get<0>(response) != Ok) {
        // TODO(devincarr): check the reason for the send failure (if needed).
        return BadDisconnect;
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
    response = sread_wait();
    // TODO(devincarr): What happens when a Timeout occurs, is it different from Disconnect?
    if (std::get<0>(response) == Timeout) {
        return BadDisconnect;
    }
    if (std::get<0>(response) == Error) {
        return BadDisconnect;
    }
    response = read_tag(std::get<1>(response),"MOVE");

    // assign the move to the game player move
    game->player_move = std::get<1>(response);

    // Everything is okay, verify the move
    return VerifyMove;
}

GCPServerSocket::ServerState GCPServerSocket::server_wait_for_other_move() {
    if (player->side == White) {
        wait_for_player(game->player2);
    } else {
        wait_for_player(game->player1);
    }

    if (game->controller->is_game_over()) {
        return GameOver;
    }

    return SendMove;
}

GCPServerSocket::ServerState GCPServerSocket::server_verify_move() {
    // check for a valid move by the user
    if (not game->controller->receive_action(game->player_move)) {
        return InvalidMove;
    }
    // the move is valid, continue to send the move to the other player
    // Set the other player as not ready before setting up them up to 
    // make a move
    if (player->side == White) {
        game->player2.ready = false;
        game->player_turn = Black;
    } else {
        game->player1.ready = false;
        game->player_turn = White;
    }
    // reset the invalid_moves for the next player
    game->invalid_moves = 0;
    // set this player's move to ready to trigger the other player to send 
    // the previous player's move.
    player->ready = true;
    game->move_cv.notify_all();

    // check for end of game
    if (game->controller->is_game_over()) {
        return GameOver;
    }

    return WaitForOtherMove;
}

GCPServerSocket::ServerState GCPServerSocket::server_send_move() {
    response = swrite("MOVE:"+game->player_move);
    // if the move cannot be send, Disconnect
    if (std::get<0>(response) != Ok) {
        return BadDisconnect;
    }

    return WaitForMove;
}

GCPServerSocket::ServerState GCPServerSocket::server_send_gameover() {
    PlayerSide winner;
    if (game->controller->get_result() == Gamelogic::Game_result::Player_one) {
        winner = White;
    } else {
        winner = Black;
    }
    if (player->side == winner) {
        response = swrite("GAMEOVER:WIN");
    } else {
        response = swrite("GAMEOVER:LOSE");
    }
    // if the move cannot be send, Disconnect
    if (std::get<0>(response) != Ok) {
        return BadDisconnect;
    }

    return GoodDisconnect;
}

// ======================================================================
// Invalid State Functions
// ======================================================================

GCPServerSocket::ServerState GCPServerSocket::server_invalid_auth_game() {
    response = swrite("AUTH:INVALIDGAME\n");
    // doesn't truly matter if the response was received, the client may
    // disconnect automatically.
    return GoodDisconnect;
}

GCPServerSocket::ServerState GCPServerSocket::server_invalid_auth_name() {
    response = swrite("AUTH:INVALIDNAME\n");
    // doesn't truly matter if the response was received, the client may
    // disconnect automatically.
    return GoodDisconnect;
}

GCPServerSocket::ServerState GCPServerSocket::server_invalid_move() {
    if (game->invalid_moves >= Morphling::ServerState::Game_instance::MAX_ATTEMPTS) {
        // the client is stuck and must disconnect and end game
        return GoodDisconnect;
    }
    // increment the total number of invalid moves
    game->invalid_moves++;

    // Send that the move was invalid
    response = swrite("MOVE:INVALID("+game->player_move+")\n");
    if (std::get<0>(response) != Ok) {
        return BadDisconnect;
    }

    return WaitForMove;
}

// Locks both of the threads at this point to create a barrier for
// the two running threads
GCPServerSocket::ServerState GCPServerSocket::barrier(GCPServerSocket::ServerState next_state) {
    std::unique_lock<std::mutex> barrier_lock(game->barrier_mutex);
    // figure who is the other player
    Player* other_player = nullptr;
    if (player->side == White) {
        //std::cout << "White and other is Black" << std::endl;
        other_player = &game->player2;
    } else {
        //std::cout << "Black and other is White" << std::endl;
        other_player = &game->player1;
    }

    // check if the other player has already reached the barrier
    if (other_player->barrier_ready) {
        //std::cout << "both ready" << std::endl;
        player->barrier_ready = true;
        // release the barrier mutex to allow the other thread to check
        // if the player is ready.
        barrier_lock.unlock();
        // notify the other player that we have connected
        game->move_cv.notify_all();
        // wait for the other user to set both barriers to false
        std::unique_lock<std::mutex> lkpost(game->move_post_mutex);
        game->move_post_cv.wait(lkpost, [&]{return not game->player1.barrier_ready and not game->player2.barrier_ready;});
    } else {
        // otherwise wait for the other player to be ready
        //std::cout << "waiting for other" << std::endl;
        player->barrier_ready = true;
        // release the barrier mutex to allow the other thread to check
        // if the player is ready.
        barrier_lock.unlock();
        // Wait for the other player to connect
        std::unique_lock<std::mutex> lk(game->move_mutex);
        game->move_cv.wait(lk, [&] {
            return other_player->barrier_ready or not _connected or not game->running;
        });
        // Set both players to false in lock-step
        game->player2.barrier_ready = false;
        game->player1.barrier_ready = false;
        // notify other player that we can both continue to next state
        game->move_post_cv.notify_all();
    }
    //std::cout << "going" << std::endl;
    
    // continue to the next state provided
    return next_state;
}

void GCPServerSocket::wait_for_player(Player& other_player) {
    std::unique_lock<std::mutex> lk(game->move_mutex);
    game->move_cv.wait(lk, [&] {
        return other_player.ready or not _connected or not game->running;
    });
}

bool GCPServerSocket::check_server_status() {
    if (game == nullptr) {
        return state != BadDisconnect and _connected;
    } else {
        return state != BadDisconnect and _connected and game->running;
    }
}

// ======================================================================
// Public Functions
// ======================================================================

void GCPServerSocket::disconnect() {
    state = GoodDisconnect;
    if (game != nullptr) {
        game->stop_game();
        game->move_cv.notify_all();
        game->move_post_cv.notify_all();
    }
    GCPSocket::disconnect();
}

void GCPServerSocket::baddisconnect() {
    if (game != nullptr) {
        game->stop_game();
        game->move_cv.notify_all();
        game->move_post_cv.notify_all();
    }
    // Print out the previous error if there was one
    if (std::get<0>(response) != Ok) {
        std::cerr << "Error: " << std::get<1>(response) << std::endl;
    }
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
                STATELOG("VerifyAuth");
                state = server_verify_auth();
                break;
            }
            case WaitForOtherConnect: {
                STATELOG("WaitForOtherConnect");
                state = barrier(SendWB);
                break;
            }
            case SendWB: {
                std::string msg = "SendWB: player: ";
                msg += (player->side==White?"W":"B");
                STATELOG(msg);
                state = server_send_side();
                break;
            }
            case WaitForMove: {
                std::string msg = (player->side==White?"W":"B");
                STATELOG("WaitForMove as "+msg);
                state = server_wait_for_move();
                break;
            }
            case WaitForOtherMove: {
                std::string msg = (player->side==White?"W":"B");
                STATELOG("WaitForOtherMove as "+msg);
                state = server_wait_for_other_move();
                break;
            }
            case VerifyMove: {
                STATELOG("VerifyMove: "+game->player_move);
                state = server_verify_move();
                break;
            }
            case SendMove: {
                std::string msg = (player->side==White?"W":"B");
                STATELOG("SendMove ("+game->player_move+") to "+msg);
                state = server_send_move();
                break;
            }
            case InvalidAuthGame: {
                STATELOG("InvalidAuthGame");
                state = server_invalid_auth_game();
                break;
            }
            case InvalidAuthName: {
                STATELOG("InvalidAuthName");
                state = server_invalid_auth_name();
                break;
            }
            case InvalidMove: {
                STATELOG("InvalidMove");
                state = server_invalid_move();
                break;
            }
            case GameOver: {
                STATELOG("GameOver");
                state = barrier(server_send_gameover());
                // fallthrough into GoodDisconnect
            }
            case GoodDisconnect: {
                STATELOG("GoodDisconnect");
                disconnect();
                return;
            }
            default:
            case BadDisconnect: {
                STATELOG("BadDisconnect");
                // A disconnect was called
                // TODO(devincarr): Verify if it was a hard disconnect or error in response
                // Assume safe disconnect and send message

                baddisconnect();
                return;
            }
        }
    }
}

