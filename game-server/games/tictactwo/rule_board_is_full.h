#pragma once

#include <gamelogic/controller/rule.h>
#include <gamelogic/model/model.h>


namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_model;

  class Rule_board_is_full : public Rule {
  public:
    bool check(Model* model) override;
    std::string get_message() const override;

  private:
    bool check_board_is_full(Tictactwo_model* model);
  };
}
