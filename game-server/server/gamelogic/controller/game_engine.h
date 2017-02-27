#pragma once

#include <model/model.h>
#include "controller.h"

namespace Morphling::Gamelogic {
  class Game_engine
  {
  public:
    virtual ~Game_engine();

    // called just before the game begins
    virtual void initialize() = 0;

    // called just after the game ends
    virtual void end() = 0;

    // begin the game
    void play();

  protected:
    void set_model(Model* model) { this->model = model; }
    void set_controller(Controller* controller) { this->controller = controller; }

  private:
    Model* model;
    Controller* controller;
  };

}
