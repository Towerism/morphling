#pragma once

#include "player.h"

namespace Morphling::Gamelogic {
  class Model {
  public:
    Model() = default;
    virtual ~Model() = default;

    virtual bool is_game_over() = 0;
    virtual Player* get_winner() = 0;

    Player *to_next_player();

    Player *get_current_player() { return current_player; }
    const Player *get_current_player() const { return current_player; }

    void set_player_one(Player *player) {
      player_one = player;
      current_player = player_one;
    }
    void set_player_two(Player *player) { player_two = player; }

  private:
    Player *current_player = nullptr;
    Player *player_one = nullptr;
    Player *player_two = nullptr;
  };
}
