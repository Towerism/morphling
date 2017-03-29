#include "rule_has_alignment.h"

#include "tictactwo_model.h"

namespace Morphling::Gamelogic::Tictactwo {

  bool Rule_has_alignment::check(Model* model) {
    bool result = false;
    auto tictactoe_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactoe_model != nullptr) {
      result = check_has_alignment(tictactoe_model);
    }
    return result;
  }

  bool Rule_has_alignment::check_has_alignment(Tictactwo_model* model) {
    return check_line(model, 0, 0, 0, 1, 0, 2) ||
      check_line(model, 1, 0, 1, 1, 1, 2) ||
      check_line(model, 2, 0, 2, 1, 2, 2) ||
      check_line(model, 0, 0, 1, 0, 2, 0) ||
      check_line(model, 0, 1, 1, 1, 2, 1) ||
      check_line(model, 0, 2, 1, 2, 2, 2) ||
      check_line(model, 0, 0, 1, 1, 2, 2) ||
      check_line(model, 2, 0, 1, 1, 0, 2);
  }

  bool Rule_has_alignment::check_line(Tictactwo_model* model, int x1, int y1,
                                      int x2, int y2, int x3, int y3) {
    return check_equal_positions(model, x1, y1, x2, y2)
      && check_equal_positions(model, x2, y2, x3, y3);
  }

  bool Rule_has_alignment::check_equal_positions(Tictactwo_model* model, int x1, int y1, int x2, int y2) {
    auto grid_origin = model->get_grid_origin();
    auto adjusted_position1 = Point2D(x1, y1).get_translation(grid_origin);
    auto adjusted_position2 = Point2D(x2, y2).get_translation(grid_origin);
    auto element1 = model->get_element(adjusted_position1);
    auto element2 = model->get_element(adjusted_position2);
    if (element1 == nullptr || element2 == nullptr)
      return false;
    return element1->equals(element2.get());
  }

  std::string Rule_has_alignment::get_message() const {
    return "Three pieces align!";
  }
}
