#include <serverstate/server_state.h>

using namespace Morphling::Database;

namespace Morphling::ServerState {

    Server_state::Server_state(Gamelogic::Game_engine* engine) : engine(engine), fb{"morphling-50028"}
    { }

    Server_state::game_instance_t Server_state::get_game(std::string gameid, std::string name) {
        // Lock the map until finished accessing
        std::unique_lock<std::mutex> lock(map_mutex);

        game_instance_t gi = nullptr;
        auto game = game_map.find(gameid);
        // if the game exists locally
        if (game != game_map.end()) {
            // assign it to the outer object
            gi = game->second;
        } else {
            // Check if the game exists in firebase
            fire_err fe = fb.get_json("games/"+gameid+".json");
            // check the error code
            if (fe.res_code != CURLE_OK) {
                // return an invalid game
                return nullptr;
            }

            // if it doesn't exist return nullptr
            if (fe.res_json == nullptr) {
                return nullptr;
            }

            // if it does exist, prepare a new Game_instance,
            // put it in the game_map cache and prepare it
            // for return to caller
            std::string player1 = fe.res_json["player1"];
            std::string player2 = fe.res_json["player2"];
            game_map.insert(std::make_pair(gameid,std::make_shared<Game_instance>(engine->initialize(player1, player2),gameid,player1,player2)));
            
            auto game_inst = game_map.find(gameid);
            if (game_inst != game_map.end()) {
                gi = game_inst->second;
            } else {
                gi = nullptr;
            }
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
