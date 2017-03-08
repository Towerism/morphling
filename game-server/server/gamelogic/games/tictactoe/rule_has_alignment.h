#pragma once

#include <gamelogic/controller/rule.h>
#include <gamelogic/model/model.h>


namespace Morphling::Gamelogic::Tictactoe {
  class Tictactoe_model;

  class Rule_has_alignment : public Rule {
  public:
    bool check(Model* model) override;
    std::string get_message() const override;

  private:
    bool check_real(Tictactoe_model* model);
    bool check_line(Tictactoe_model* model, int x1, int y1, int x2, int y2,
                    int x3, int y3);
    bool check_equal_positions(Tictactoe_model* model, int x1, int x2, int y1,
                               int y2);
  };
}
