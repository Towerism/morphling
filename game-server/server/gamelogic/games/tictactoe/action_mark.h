#include <gamelogic/controller/action.h>
#include <gamelogic/model/model.h>
#include <gamelogic/model/player.h>

namespace Morphling::Gamelogic::Tictactoe {
  class Tictactoe_model;

  class Action_mark : public Action {
  public:
    Action_mark(int x, int y) : x(x), y(y) {}

    bool is_legal(Model* model) override;
    void execute(Model* model) override;
  private:
    int x, y;

    bool is_space_free(Tictactoe_model* model);
    void mark_space(Tictactoe_model* model);
  };
}

