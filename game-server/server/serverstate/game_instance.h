#pragma once

#include <gamelogic/controller/game_engine.h>
#include <gamelogic/controller/controller.h>

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
    Player(std::string n, PlayerSide ps) : name(n), ready(false), side(ps) {}

    std::string name;
    bool ready;
    PlayerSide side;
  };

  class Game_instance {
  public:
    typedef Gamelogic::Controller* controller_t;

    Game_instance(controller_t c, std::string gameid, std::string name1, std::string name2);

    std::atomic_bool running;
    std::string gameid;
    Player player1; // White
    Player player2; // Black

    std::mutex move_mutex;
    std::condition_variable move_cv;
    std::mutex move_post_mutex;
    std::condition_variable move_post_cv;
    PlayerSide player_turn;
    controller_t controller;
    std::string player_move;

    std::tuple<bool,Player*> check_player_name(std::string name);
    void stop_game();
  };
}
