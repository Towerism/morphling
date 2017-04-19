#pragma once

#include <gamelogic/controller/game_engine.h>
#include <gamelogic/controller/controller.h>

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <tuple>

namespace Morphling::ServerState {
  enum PlayerSide {
    White,
    Black
  };

  class Player {
  public:
    Player(std::string n, PlayerSide ps):
      name(n), ready(false), barrier_ready(false), side(ps) {}

    std::string name;
    std::atomic_bool ready;
    std::atomic_bool barrier_ready;
    PlayerSide side;
  };

  class Game_instance {
  public:
    typedef Gamelogic::Controller* controller_t;
    static const size_t MAX_ATTEMPTS = 2;

    Game_instance(controller_t c, std::string gameid, std::string name1, std::string name2);

    controller_t controller;
    std::atomic_bool running;
    std::atomic_bool players_connected;

    std::string gameid;
    Player player1; // White
    Player player2; // Black

    // State variables for lock-step
    std::mutex barrier_mutex;
    std::mutex move_mutex;
    std::condition_variable move_cv;
    std::mutex move_post_mutex;
    std::condition_variable move_post_cv;

    // The player's current move
    PlayerSide player_turn;
    std::string player_move;
    size_t invalid_moves;

    std::tuple<bool,Player*> check_player_name(std::string name);
    void stop_game();
  };
}
