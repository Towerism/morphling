#include "action_move_grid.h"

#include "tictactwo_model.h"

namespace Morphling::Gamelogic::Tictactwo {

  Action_move_grid::Action_move_grid(Direction direction) : direction(direction) {}

  bool Action_move_grid::is_legal(Model* model) {
    auto tictactwo_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactwo_model != nullptr) {
      return is_direction_valid(tictactwo_model);
    }
    return false;
  }

  bool Action_move_grid::is_direction_valid(Tictactwo_model* model) {
    auto origin = new_grid_origin_from_direction(model);
    return origin.obeys_dimensions({ 5, 5 });
  }

  Point2D Action_move_grid::new_grid_origin_from_direction(Tictactwo_model* model) {
    Point2D point = model->get_grid_origin();
    switch(direction) {
    case Direction::Up:
      point.translate({ 0, -1 });
      break;
    case Direction::Down:
      point.translate({ 0, 1 });
      break;
    case Direction::Left:
      point.translate({ -1, 0 });
      break;
    case Direction::Right:
      point.translate({ 1, 0 });
      break;
    case Direction::Up_left:
      point.translate({ -1, -1 });
      break;
    case Direction::Up_right:
      point.translate({ 1, -1 });
      break;
    case Direction::Down_left:
      point.translate({ -1, 1 });
      break;
    case Direction::Down_right:
      point.translate({ 1, 1 });
      break;
    }
    return point;
  }

  void Action_move_grid::execute(Model* model) {
    auto tictactwo_model = dynamic_cast<Tictactwo_model*>(model);
    if (tictactwo_model != nullptr) {
      move_grid(tictactwo_model);
    }
  }

  void Action_move_grid::move_grid(Tictactwo_model* model) {
    auto origin = new_grid_origin_from_direction(model);
    model->set_grid_origin(origin);
  }

}
