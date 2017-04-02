#include <unordered_map>
#include <memory>

#include "game_instance.h"

namespace Morphling::ServerState {

  class Server_state {
  public:
    typedef std::unique_ptr<Game_instance> game_instance_t;
    typedef std::shared_ptr<Gamelogic::Game_engine> engine_t;

    Server_state(Gamelogic::Game_engine* engine) : engine(engine) {}

    Game_instance* get_game(std::string id);
    void end_game(std::string id);

  private:
    std::unordered_map<std::string, game_instance_t> game_map;
    engine_t engine;
  };
}
