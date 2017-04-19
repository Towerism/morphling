#include <serverstate/server_state.h>

namespace Morphling::ServerState {

    Server_state::Server_state(Gamelogic::Game_engine* engine) : engine(engine) {
        // TODO(devincarr): remove the demo game from the map
        std::string player1 = "player1";
        std::string player2 = "player2";
        std::string _game = "game";
        game_map.insert(std::make_pair(_game,std::make_shared<Game_instance>(engine->initialize(player1, player2),_game,player1,player2)));
    }

    Server_state::game_instance_t Server_state::get_game(std::string gameid, std::string name) {
        game_instance_t gi = nullptr;
        auto game = game_map.find(gameid);
        // if the game exists locally
        if (game != game_map.end()) {
            // assign it to the outer object
            gi = game->second;
        } else {
            // else check for the game if it exists in firebase
            // TODO(devincarr)

            // if it doesn't exist return nullptr

            // if it does exist, prepare a new Game_instance,
            // put it in the game_map cache and prepare it
            // for return to caller
            // TODO(devincarr): remove the dummy game object later.
            
            //gi = game_map[_game];
        }
        
        // return the game
        return gi;
    }

    void Server_state::end_game(std::string id) {

    }

    void Server_state::disconnect_all_games() {
        for (auto& game : game_map) {
            game.second->stop_game();
        }
    }
}
