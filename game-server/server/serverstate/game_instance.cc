#include <serverstate/game_instance.h>

namespace Morphling::ServerState {

    Game_instance::Game_instance(controller_t c, std::string id, std::string name1, std::string name2) :
        controller(c),
        running(true),
        players_connected(false),
        gameid(id),
        player1{name1,White},
        player2{name2,Black},
        player_turn(White),
        invalid_moves(0),
        prev_move_time(std::chrono::steady_clock::now())
    { }

    std::tuple<bool,Player*> Game_instance::check_player_name(std::string name) {
        if (name == "player1") {
            return std::make_tuple(true,&player1);
        } else if (name == "player2") {
            return std::make_tuple(true,&player2);
        } else {
            return std::make_tuple(false,nullptr);
        }
    }

    void Game_instance::stop_game() {
        if (running) {
            running = false;
            move_cv.notify_all();
        }
    }
}