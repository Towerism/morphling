#include "rule_has_alignment.h"

#include "tictactoe_model.h"

namespace Morphling::Gamelogic::Tictactoe {

  bool Rule_has_alignment::check(Model* model) {
    bool result = false;
    auto tictactoe_model = dynamic_cast<Tictactoe_model*>(model);
    if (tictactoe_model != nullptr) {
      result = check_has_alignment(tictactoe_model);
    }
    return result;
  }

  bool Rule_has_alignment::check_has_alignment(Tictactoe_model* model) {
    return check_line(model, 0, 0, 0, 1, 0, 2) ||
      check_line(model, 1, 0, 1, 1, 1, 2) ||
      check_line(model, 2, 0, 2, 1, 2, 2) ||
      check_line(model, 0, 0, 1, 0, 2, 0) ||
      check_line(model, 0, 1, 1, 1, 2, 1) ||
      check_line(model, 0, 2, 1, 2, 2, 2) ||
      check_line(model, 0, 0, 1, 1, 2, 2) ||
      check_line(model, 2, 0, 1, 1, 0, 2);
  }

  bool Rule_has_alignment::check_line(Tictactoe_model* model, int x1, int y1,
                                      int x2, int y2, int x3, int y3) {
    return model->get_element({x1, y1}) != nullptr
      && model->get_element({x2, y2}) != nullptr
      && model->get_element({x3, y3}) != nullptr
      && check_equal_positions(model, x1, y1, x2, y2)
      && check_equal_positions(model, x2, y2, x3, y3);
  }

  bool Rule_has_alignment::check_equal_positions(Tictactoe_model* model, int x1, int y1, int x2, int y2) {
    return model->get_element({x1, y1})->equals(model->get_element({x2, y2}).get());
  }

  std::string Rule_has_alignment::get_message() const {
    return "Three pieces align!";
  }
}
