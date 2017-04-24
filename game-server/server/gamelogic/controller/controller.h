#pragma once

#include <memory>
#include <string>

#include <gamelogic/model/model.h>
#include <gamelogic/model/player.h>
#include <gamelogic/view/model_to_strings_mapper.h>
#include <gamelogic/view/token_to_url_mapper.h>
#include "action.h"

namespace Morphling::Gamelogic {
  class Controller {
  public:
    typedef std::shared_ptr<Action> action_t;

    Controller(Model* model) : model(model) {}
    virtual ~Controller() { if (model_serializer) delete model_serializer; };

    virtual action_t parse_action(std::string action_string) = 0;

    bool is_game_over() { return model->is_game_over(); }

    Game_result get_result() { return model->get_result(); };

    // Returns true if action was received successfully
    // Returns false otherwise.
    // Action can be recieved successfully if the action_string is
    // malformed or the parsed action is illegal.
    bool receive_action(std::string action_string);

    const Player* get_current_player() const { return model->get_current_player(); }
    Player* get_current_player() { return model->get_current_player(); }

    Model_to_strings_mapper::strings_t serialize_model() { return model_serializer->get_strings(); }
    void set_model_serializer(Model_to_strings_mapper* mapper) { model_serializer = mapper; }
    std::string map_token_to_url(char token) const { return token_to_url_mapper.get_url(token); }
    void add_url(char token, std::string url) { token_to_url_mapper.add_url(token, url); }


  protected:
    virtual void execute_action(action_t action) = 0;
    Model* model;

  private:
    Token_to_url_mapper token_to_url_mapper;
    Model_to_strings_mapper* model_serializer;
  };

}
