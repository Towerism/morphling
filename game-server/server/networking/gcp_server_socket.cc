#include <networking/gcp_server_socket.h>

using namespace Morphling::Networking;

GCPServerSocket::GCPServerSocket(int sockfd): _sockfd(sockfd), _connected(true)
{ }


GCPServerSocket::~GCPServerSocket() {
    disconnect();
}

// ======================================================================
// Socket Direct Access Functions
// ======================================================================

std::tuple<GCPServerSocket::SocketReturn,std::string>
GCPServerSocket::sread()
{
    char buffer[MAX_MESSAGE];

    memset(buffer, 0, MAX_MESSAGE);

    struct timeval r_timeout;
    r_timeout.tv_sec = 2;
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

std::tuple<GCPServerSocket::SocketReturn,std::string>
GCPServerSocket::swrite(std::string msg)
{
    if (_connected) {
        if (send(_sockfd, msg.c_str(), strlen(msg.c_str()), 0) == -1) {
            return std::make_tuple(Error,std::string(strerror(errno)));
        }
    }

    return std::make_tuple(Ok,"");
}

// Read a line from the GCP socket and parse the sent Tag.
// Example socket read: "AUTH:asdf"
// will return "asdf" for input of Tag: "AUTH"
RET GCPServerSocket::read_tag(std::string tag) {
    auto response = sread();
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

// Send a proper "BYE" to before closing the connection
void GCPServerSocket::send_close() {
    // Send the disconnect code: BYE
    auto ret = swrite("BYE");
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
    auto auth = read_tag("AUTH");
    // if the read_tag failed, disconnect
    if (std::get<0>(auth)) return Disconnect;
    // TODO(devincarr): process the AUTH and establish to the proper game server
    playerid = std::get<1>(auth);
    std::cout << "Auth receieved: " << playerid << std::endl;
    
    return WaitForOtherConnect;
}

// Wait for the next player to join. Or if this is the last player to 
// to join, continue.
GCPServerSocket::ServerState GCPServerSocket::server_wait_for_other(players_ready* rdy) {
    std::unique_lock<std::mutex> lk(m_player);
    // check if the other player has already connected
    if (rdy->player1) {
        rdy->player2 = true;
        return SendWB;
    }
    // otherwise wait for the other player to be ready
    cv_player.wait(lk, [rdy,this]{return rdy->player2 or not _connected;});
    // If the reason for not waiting is not connected => Disconnect
    if (_connected) return Disconnect;
    else return SendWB;
}

// Send this player which side they are on based on the tournament data.
// Then wait for either this player, or the other player to make a move
// (white moves first).
GCPServerSocket::ServerState GCPServerSocket::server_send_side() {
    // send the side 
    std::string side;
    if (playerside == White) side = "W"; else side = "B";
    auto ret = swrite("SIDE:"+side);

    // check the send status
    if (std::get<0>(ret) != Ok) {
        // TODO(devincarr): check the reason for the send failure (if needed).
        return Disconnect;
    }

    // transition to the wait step for the move if PlayerSide::White
    if (playerside == White) {
        return WaitForMove;
    } else {
        // otherwise wait for the other player to make a move
        return WaitForOtherMove;
    }
}

// ======================================================================
// Public Functions
// ======================================================================

void GCPServerSocket::disconnect() {
    if (_connected) {
        close(_sockfd);
        _connected = false;
    }
}

// Entry point for the start of the server state response state machine
void GCPServerSocket::start() {
    ServerState state = VerifyAuth;
    // enter the state machine
    while (state != Disconnect && _connected) {
        // process and transistion to the next state
        switch (state) {
            case VerifyAuth: {
                state = server_verify_auth();
                break;
            }
            case WaitForOtherConnect: {
                //state = server_wait_for_other();
                //break;
            }
            case SendWB: {
                //state = server_send_side();
                //break;
            }
            case WaitForMove: {
                // TODO(devincarr): fall through for now
            }
            case WaitForOtherMove: {
                // TODO(devincarr): fall through for now
            }
            case VerifyMove: {
                // TODO(devincarr): fall through for now
            }
            default:
            case Disconnect: {
                // exit the state machine for the Disconnect
                break;
            }
        }
    }
    
    // A disconnect was called
    // TODO(devincarr): Verify if it was a hard disconnect or error in response
    // Assume safe disconnect and send message
    send_close();
    disconnect();
}

