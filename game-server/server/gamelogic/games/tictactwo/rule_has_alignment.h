#pragma once

#include <gamelogic/controller/rule.h>
#include <gamelogic/model/model.h>


namespace Morphling::Gamelogic::Tictactwo {
  class Tictactwo_model;

  class Rule_has_alignment : public Rule {
  public:
    bool check(Model* model) override;
    std::string get_message() const override;

  private:
    bool check_has_alignment(Tictactwo_model* model);
    bool check_line(Tictactwo_model* model, int x1, int y1, int x2, int y2,
                    int x3, int y3);
    bool check_equal_positions(Tictactwo_model* model, int x1, int x2, int y1,
                               int y2);
  };
}
