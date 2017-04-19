#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>

#include <serverstate/game_instance.h>
#include <database/firebase.h>

using namespace Morphling::Database;

namespace Morphling::ServerState {

  class Server_state {
  public:
    typedef std::shared_ptr<Game_instance> game_instance_t;
    typedef std::shared_ptr<Gamelogic::Game_engine> engine_t;

    Server_state(Gamelogic::Game_engine* engine);

    // Given a game id string, search for the game if it exists 
    // locally in the game_map cache or in firebase. If it exists 
    // in firebase but not locally, create it, store it, and 
    // return it.
    game_instance_t get_game(std::string gameid, std::string name);
    void end_game(std::string id);
    void disconnect_all_games();

  private:
    std::mutex map_mutex;
    std::unordered_map<std::string, game_instance_t> game_map;
    engine_t engine;

    Database::firebase fb;
  };
}
