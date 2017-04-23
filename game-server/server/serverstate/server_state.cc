#include <serverstate/server_state.h>

using namespace Morphling::Database;

namespace Morphling::ServerState {

    Server_state::Server_state(Gamelogic::Game_engine* engine) : engine(engine), fb{"morphling-50028"}
    { }

    Server_state::game_instance_t Server_state::get_game(std::string gameid, std::string name) {
        // Lock the map until finished accessing
        std::unique_lock<std::mutex> lock(state_mutex);

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

            json game = fe.res_json;

            // load the player list from firebase
            fe = fb.get_json("players.json");
            // check the error code
            if (fe.res_code != CURLE_OK) {
                // return an invalid game
                return nullptr;
            }
            json players = fe.res_json;

            // if it does exist, prepare a new Game_instance,
            // put it in the game_map cache and prepare it
            // for return to caller
            std::string player1 = players[game["player1"].get<std::string>()]["name"];
            std::string player2 = players[game["player2"].get<std::string>()]["name"];
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

    void Server_state::end_game(std::string id, PlayerSide winner) {
        // Lock the Server_state until finished accessing firebase and/or game_map
        std::unique_lock<std::mutex> lock(state_mutex);

        // get the game from firebase
        fire_err fe_game = fb.get_json("games/"+id+".json");
        // check the error code
        if (fe_game.res_code != CURLE_OK) {
            // failed to load
            return;
        }

        std::string winner_string;
        if (winner == White) {
            winner_string = "player1";
            
        } else {
            winner_string = "player2";
        }

        // get the winner (player) from firebase
        std::string player_hash = fe_game.res_json[winner_string];
        fire_err fe_player = fb.get_json("players/"+player_hash+".json");
        // check the error code
        if (fe_player.res_code != CURLE_OK) {
            // failed to load
            return;
        }

        // set the winner of the game for the game object in firebase
        json updated_game = fe_game.res_json;
        updated_game["score"] = winner_string;
        fire_err fe = fb.write_json("games/"+id+".json",updated_game);
        if (fe.res_code != CURLE_OK) {
            // failed to save score
            return;
        }
        // update the score of the player
        json updated_player = fe_player.res_json;
        updated_player["score"] = fe_player.res_json["score"].get<int>() + 1;
        fe = fb.write_json("players/"+player_hash+".json",updated_player);
        if (fe.res_code != CURLE_OK) {
            // failed to save score
            return;
        }
    }

    void Server_state::disconnect_all_games() {
        // Lock the map until finished accessing
        std::unique_lock<std::mutex> lock(state_mutex);

        for (auto& game : game_map) {
            game.second->stop_game();
        }
    }
}
