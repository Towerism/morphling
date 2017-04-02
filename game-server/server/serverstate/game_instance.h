#include <gamelogic/controller/game_engine.h>
#include <gamelogic/controller/controller.h>

#include <memory>

namespace Morphling::ServerState {

  class Game_instance {
  public:
    typedef std::shared_ptr<Gamelogic::Game_engine> engine_t;
    typedef std::unique_ptr<Gamelogic::Controller> controller_t;

    Game_instance(engine_t engine) : engine(engine) {}

    bool players_ready;
    engine_t engine;
    controller_t controller;
  };
}
