#pragma once

#include <atomic>
#include <chrono>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>

#include <serverstate/game_instance.h>
#include <database/firebase.h>
#include <networking/gcp_socket.h>

using namespace Morphling::Database;

namespace Morphling::ServerState {

  class Server_state {
  public:
    typedef std::shared_ptr<Game_instance> game_instance_t;
    typedef std::shared_ptr<Gamelogic::Game_engine> engine_t;

    Server_state(Gamelogic::Game_engine* engine, std::string db);

    // Given a game id string, search for the game if it exists 
    // locally in the game_map cache or in firebase. If it exists 
    // in firebase but not locally, create it, store it, and 
    // return it.
    game_instance_t get_game(std::string gameid);

    // End the current game and increment the score of the winning 
    // player in firebase.
    void end_game(std::string id, PlayerSide winner);

    // Given a board state, push the value to the game's 
    // state object in firebase.
    void send_move(std::string gameid, std::vector<std::string> board_state);

    // Disconnect the rest of the games that are currently running 
    // in the game_map
    void disconnect_all_games();

    // Atomic public variables for server settings
    std::atomic<size_t> tries;
    std::atomic<time_t> sock_wait;

    std::chrono::seconds get_delay();

  private:
    std::mutex state_mutex;
    std::unordered_map<std::string, game_instance_t> game_map;
    engine_t engine;

    Database::firebase fb;

    std::mutex delay_mutex;
    std::chrono::seconds delay_duration;

    // lock-safe access to set the delay duration
    void set_delay(size_t seconds);

    // Check firebase for settings variables to update
    void update_settings();
  };
}
