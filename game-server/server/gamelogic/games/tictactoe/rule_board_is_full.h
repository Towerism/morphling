#pragma once

#include <controller/rule.h>
#include <model/model.h>


namespace Morphling::Gamelogic::Tictactoe {
  class Tictactoe_model;

  class Rule_board_is_full : public Rule {
  public:
    bool check(Model* model) override;
    std::string get_message() const override;

  private:
    bool check_real(Tictactoe_model* model);
  };
}
