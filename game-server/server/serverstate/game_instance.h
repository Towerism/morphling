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
    Player(std::string n) : name(n), ready(false) {}

    std::string name;
    bool ready;
    PlayerSide side;
  };

  class Game_instance {
  public:
    typedef Gamelogic::Controller* controller_t;

    Game_instance(controller_t c, std::string gameid, std::string name1, std::string name2);

    std::string gameid;
    Player player1; // White
    Player player2; // Black
    std::mutex move_mutex;
    std::condition_variable move_cv;
    controller_t controller;

    std::tuple<bool,Player*> check_player_name(std::string name);
  };
}
