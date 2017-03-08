#include <controller/action.h>
#include <model/model.h>
#include <model/player.h>

namespace Morphling::Gamelogic::Tictactoe {
  class Tictactoe_model;

  class Action_mark : public Action {
  public:
    Action_mark(int x, int y) : x(x), y(y) {}

    bool is_legal(Model* model) override;
    void execute(Model* model) override;
  private:
    int x, y;

    bool is_legal_real(Tictactoe_model* model);
    void execute_real(Tictactoe_model* model);
  };
}

