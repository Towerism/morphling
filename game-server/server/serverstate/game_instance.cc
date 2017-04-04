#include "game_instance.h"

namespace Morphling::ServerState {

    Game_instance::Game_instance(controller_t c, std::string id, std::string name1, std::string name2) :
        gameid(id),
        player1(Player(name1)),
        player2(Player(name2)),
        controller(c)
    { }

    std::tuple<bool,Player*> Game_instance::check_player_name(std::string name) {
        if (name == player1.name) {
            return std::make_tuple(true,&player1);
        } else if (name == player2.name) {
            return std::make_tuple(true,&player2);
        } else {
            return std::make_tuple(false,nullptr);
        }
    }
}