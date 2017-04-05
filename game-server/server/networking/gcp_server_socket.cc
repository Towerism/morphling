#include <networking/gcp_server_socket.h>

using namespace Morphling::Networking;
using namespace Morphling::ServerState;

GCPServerSocket::GCPServerSocket(Server_state* ss, int sockfd): 
    _sockfd(sockfd),
    _connected(true),
    serverstate(ss),
    max_tries(3)
{ }


GCPServerSocket::~GCPServerSocket() {
    disconnect();
}

// ======================================================================
// Socket Direct Access Functions
// ======================================================================

GCPServerSocket::RET GCPServerSocket::sread()
{
    char buffer[MAX_MESSAGE];

    memset(buffer, 0, MAX_MESSAGE);

    struct timeval r_timeout;
    r_timeout.tv_sec = 4;
    r_timeout.tv_usec = 0;
    
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(_sockfd, &read_fd);

    int rv = select(_sockfd+1, &read_fd, nullptr, nullptr, &r_timeout);
    if (rv > 0) {
        int res = recv(_sockfd, buffer, MAX_MESSAGE - 2, 0);
        if (res > 0) {
            std::string read_s = std::string(buffer);
            // normal read
            return std::make_tuple(Ok,read_s);
        } else {
            // bad read
            return std::make_tuple(Error,"Error: Bad socket read");
        }
    } else if (rv == 0) {
        // timeout occured
        return std::make_tuple(Timeout,"");
    } else {
        // error occured
        return std::make_tuple(Error,"Error: Socket select returned "+std::to_string(rv));
    }
}

GCPServerSocket::RET GCPServerSocket::sread_wait(time_t seconds)
{
    char buffer[MAX_MESSAGE];
    memset(buffer, 0, MAX_MESSAGE);

    // make sure to set an upper bound for the MAX_TIMEOUT
    if (seconds > MAX_TIMEOUT) {
        seconds = MAX_TIMEOUT;
    }

    struct timeval r_timeout;
    r_timeout.tv_sec = seconds;
    r_timeout.tv_usec = 0;

    fd_set read_fd;

    int tries = max_tries;

    while (tries > 0) {
    
        FD_ZERO(&read_fd);
        FD_SET(_sockfd, &read_fd);

        int rv = select(_sockfd+1, &read_fd, nullptr, nullptr, &r_timeout);
        if (rv > 0) {
            int res = recv(_sockfd, buffer, MAX_MESSAGE - 2, 0);
            if (res > 0) {
                std::string read_s = std::string(buffer);
                std::cerr << "Msg: " << read_s << std::endl;
                // normal read
                return std::make_tuple(Ok,read_s);
            } else {
                // bad read
                return std::make_tuple(Error,"Error: Bad socket read");
            }
        } else if (rv == 0) {
            // timeout occured but we still want to wait for a response
            tries--;
        } else {
            // error occured
            return std::make_tuple(Error,"Error: Socket select returned "+std::to_string(rv));
        }
    }
    return std::make_tuple(Timeout,"MAX_TIMEOUT: "+std::to_string(MAX_TIMEOUT));
}

GCPServerSocket::RET GCPServerSocket::swrite(std::string msg)
{
    if (_connected) {
        if (send(_sockfd, msg.c_str(), strlen(msg.c_str()), 0) == -1) {
            return std::make_tuple(Error,std::string(strerror(errno)));
        }
    } else return std::make_tuple(Error,"Disconnected");

    return std::make_tuple(Ok,"");
}

// Read a line from the GCP socket and parse the sent Tag.
// Example socket read: "AUTH:asdf"
// will return "asdf" for input of Tag: "AUTH"
GCPServerSocket::RET GCPServerSocket::read_tag(std::string tag) {
    auto response = sread_wait();
    // check if the response is Ok
    if (std::get<0>(response) != Ok)
        return std::make_tuple(Error,std::get<1>(response));
    
    std::string msg = std::get<1>(response);
    size_t colon = msg.find(":");
    if (colon == std::string::npos)
        return std::make_tuple(Error,"Error: no tag sent");

    // parse the received tag
    std::string recv_tag = msg.substr(0,colon);
    std::string value = msg.substr(colon+1);
    // verify the tag
    if (recv_tag == tag) {
        // return the value
        return std::make_tuple(Ok,value);
    } else {
        // wrong tag received
        return std::make_tuple(Error,"Error: invalid tag("+tag+"), recv: "+recv_tag);
    }
}

// Read a line from the GCP socket and parse the sent Tag.
// Example socket read: "AUTH:asdf"
// will return "asdf" for input of Tag: "AUTH"
GCPServerSocket::RTAGS GCPServerSocket::read_tags(std::string tag1, std::string tag2) {
    auto response = sread_wait(20);
    // check if the response is Ok
    if (std::get<0>(response) != Ok)
        return std::make_tuple(Error,std::get<1>(response),"");
    
    std::string msg = std::get<1>(response);
    // find the first colon
    size_t colon1 = msg.find(":");
    if (colon1 == std::string::npos)
        return std::make_tuple(Error,"Error: no tags","");
    // find the ~ divider
    size_t sq = msg.find("~",colon1+1);
    if (sq == std::string::npos)
        return std::make_tuple(Error,"Error: not two msgs sent","");
    // find the second colon
    size_t colon2 = msg.find(":",sq+1);
    if (colon2 == std::string::npos)
        return std::make_tuple(Error,"Error: no second tag","");

    size_t bn = msg.find("\n",colon2+1); 

    // parse the received tag
    std::string rtag1 = msg.substr(0,colon1);
    std::string val1 = msg.substr(colon1+1,sq-colon1-1);
    std::string rtag2 = msg.substr(sq+1,colon2-sq-1);
    std::string val2;
    if (bn == std::string::npos) {
        // if they didn't end with \n
        val2 = msg.substr(colon2+1);
    } else {
        // if they did end with \n
        val2 = msg.substr(colon2+1,bn-colon2-1);
    }
    // verify the tag
    if (rtag1 == tag1 and rtag2 == tag2) {
        // return the value
        return std::make_tuple(Ok,val1,val2);
    } else {
        // wrong tag received
        return std::make_tuple(Error,"Error: invalid tag1("+tag1+"=/="+rtag1+") or tag2("+tag2+"=/="+rtag2+")","");
    }
}

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
    std::cout << "Waiting for AUTH" << std::endl;
    auto auth = read_tags("GAME","NAME");
    std::cout << "done reading tags for AUTH" << std::endl;
    // if the read_tag failed, disconnect
    if (std::get<0>(auth) != Ok) {
        std::cout << std::get<1>(auth) << std::endl;
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
    // set the ready for the player
    //player->ready = true;
    if (player->side == White) {
        game->player1.ready = true;
        if (player->ready != game->player1.ready) {
            std::cout << "error player White is " << player->side << " (expected:"
                    << game->player1.ready << ")" << std::endl;
        }
    } else {
        game->player2.ready = true;
        if (player->ready != game->player2.ready) {
             std::cout << "error player Black is " << player->side << " (expected:"
                    << game->player2.ready << ")" << std::endl;
        }
    }

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
            std::cout << "both ready 1" << std::endl;
            game->move_cv.notify_all();
            std::unique_lock<std::mutex> lkpost(game->move_post_mutex);
            game->move_post_cv.wait(lkpost, [&]{return not game->player1.ready and not game->player2.ready;});
        } else {
            // otherwise wait for the other player to be ready
            std::cout << "waiting for B" << std::endl;
            wait_for_player(game->player2);
            game->player2.ready = false;
            game->player1.ready = false;
            game->move_post_cv.notify_all();
        }
    } else {
        if (game->player1.ready) {
            std::cout << "both ready 2" << std::endl;
            game->move_cv.notify_all();
            std::unique_lock<std::mutex> lkpost(game->move_post_mutex);
            game->move_post_cv.wait(lkpost, [&]{return not game->player1.ready and not game->player2.ready;});
        } else {
            // otherwise wait for the other player to be ready
            std::cout << "waiting for W" << std::endl;
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

bool GCPServerSocket::wait_for_player(Player& other_player) {
    std::unique_lock<std::mutex> lk(game->move_mutex);
    game->move_cv.wait(lk, [&] {
        return other_player.ready or not _connected or not game->running;
    });
}

bool GCPServerSocket::check_server_status(ServerState state) {
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
    if (_connected) {
        _connected = false;
    }
    close(_sockfd);
}

// Entry point for the start of the server state response state machine
void GCPServerSocket::start() {
    ServerState state = VerifyAuth;
    // enter the state machine
    while (check_server_status(state)) {
        // process and transistion to the next state
        switch (state) {
            case VerifyAuth: {
                std::cout << "VerifyAuth" << std::endl;
                state = server_verify_auth();
                break;
            }
            case WaitForOtherConnect: {
                std::cout << "WaitForOtherConnect" << std::endl;
                state = server_wait_for_other();
                break;
            }
            case SendWB: {
                //state = server_send_side();
                std::cout << "player: " << player->side << ", ready to send WB" << std::endl;
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
                std::cout << "Disconnect" << std::endl;
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

