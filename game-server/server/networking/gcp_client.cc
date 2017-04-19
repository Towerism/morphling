#include <networking/gcp_client.h>

using namespace Morphling::Networking;

GCPClient::GCPClient() : GCPSocket(), gameover(false), winner(false) {}

GCPClient::~GCPClient() {}

// ======================================================================
// Private Functions
// ======================================================================

bool GCPClient::check_gameover(std::string input) {
    auto ret = read_tag(input,"GAMEOVER");
    if (std::get<0>(ret) == Ok) {
        // The GAMEOVER is a part of the input
        // set the game to over and the winner
        gameover = true;
        if (std::get<1>(ret) == "WIN") {
            winner = true;
        } else {
            winner = false;
        }
        return true;
    } else {
        // The GAMEOVER is not in the input
        return false;
    }
}

// ======================================================================
// Public Functions
// ======================================================================

GCPSocket::RET GCPClient::send_auth(std::string gameid, std::string name) {
    // make sure we are still connected
    if (_connected) {
        return swrite("GAME:"+gameid+"~NAME:"+name+"\n");
    }

    return std::make_tuple(Error,"Not Connected");
}

GCPSocket::RET GCPClient::recv_auth() {
    // make sure we are still connected
    if (_connected) {
        auto ret = sread_wait(2,GCPSocket::MAX_TIMEOUT);
        if (std::get<0>(ret) != Ok) {
            return ret;
        } else {
            if (check_gameover(std::get<1>(ret))) {
                return ret;
            } else {
                return read_tag(std::get<1>(ret),"AUTH");
            }
        }
    }

    return std::make_tuple(Error,"Not Connected");
}

GCPSocket::RET GCPClient::recv_side() {
    // make sure we are still connected
    if (_connected) {
        auto ret = sread_wait(2,GCPSocket::MAX_TIMEOUT);
        if (std::get<0>(ret) != Ok) {
            return ret;
        } else {
            if (check_gameover(std::get<1>(ret))) {
                return ret;
            } else {
                return read_tag(std::get<1>(ret),"SIDE");
            }
        }
    }

    return std::make_tuple(Error,"Not Connected");
}

GCPSocket::RET GCPClient::send_move(std::string move) {
    // make sure we are still connected
    if (_connected) {
        return swrite("MOVE:"+move+"\n");
    }

    return std::make_tuple(Error,"Not Connected");
}

GCPSocket::RET GCPClient::recv_move() {
    // make sure we are still connected
    if (_connected) {
        auto ret = sread_wait(2,GCPSocket::MAX_TIMEOUT);
        if (std::get<0>(ret) != Ok) {
            return ret;
        } else {
            if (check_gameover(std::get<1>(ret))) {
                return ret;
            } else {
                return read_tag(std::get<1>(ret),"MOVE");
            }
        }
    }

    return std::make_tuple(Error,"Not Connected");
}