# Game Logic
The game logic layer is a framework that allows you to write the logic for your
board game in a very object oriented manner.

## High-level Overview
The three main components of any game are the `Game_engine`, `Controller`, and
the `Model`.

### Game_engine
The engine is responsible for constructing the model and the controller
components, mapping game object tokens to asset urls, as well as defining the
algorithm by which the model is serialized.

### Controller
The controller is primarily responsible for parsing and validating moves. When a
move is parsed and validated successfully, a corresponding `Action` is created
and is executed on the model. The controller specifies the interface for
creating these `Actions`.

### Model
The model encapsulates the main game logic for the board game. The model makes
use of `Rules` for monitoring the game state and determining who the winner is
after a game is over. 

## In-depth
Putting together a complete board game starts with the engine and ends with the
controller and the model. The engine creates the player objects which are given
to the model, the mapping between game object and image url, the algorithm for
serializing the model, and the controller.

The model's main job is simple: to monitor the game state in between moves. It
does this via `Rules`. Rules simply encapsulate checks on the model to determine
whether a particular game condition is met.

The controller's job is to receive actions in form of a string, parse the
strings into actions, and execute those actions on the model. 

For simple examples of games created using this framework, see the
`game-server/games` directory.

## Game_engine
The engine must initialize the model, controller, and model serialization
algorithm. It must also construct the players and give them to the model. All of
this is done in the `initialize` method of `Game_engine`. `Tictactwo_engine` is
an example subclass of `Game_engine`. Here is what its `initialize` function
look like:

```c++
Controller* Tictactwo_engine::initialize(std::string p1_name, std::string p2_name) {
  auto p1 = new Tictactwo_player(p1_name, new Game_object('X'));
  auto p2 = new Tictactwo_player(p2_name, new Game_object('O'));

  token_to_url_mapper.add_url('X', "/assets/X.png");
  token_to_url_mapper.add_url('O', "/assets/O.png");

  Tictactwo_model* model = new Tictactwo_model({0, 0});
  Tictactwo_controller* controller = new Tictactwo_controller(model);

  controller->set_model_serializer(new Board2D_to_strings_mapper(model->get_board()));

  model->set_player_one(p1);
  model->set_player_two(p2);

  return controller;
}
``` 

As you can see, the players are initialized with player 1 controlling 'X' and
player 2 controlling 'O'. The players are passed to the controller at the end of
the function. Then, the mapping is provided that specifies the urls for the
images of the X and O game objects. They are used in the web front end for
rendering game states. Next, the model and controller are constructed. Finally
the algorithm for serializing the model is specified. Since tictactwo represents
board state using `Board2D` it specifies the built in
`Board2D_to_strings_mapper` as the serialization algorithm.

## Model
The model should override the `is_game_over()` and `check_win_condition()`
methods and optionally the `get_result()` method. By default `get_result()`
designates the player who made the game-ending move as the winner. This may not
be desirable as is the case in tictactwo in which moving the grid into a
position which would give the other player three-in-a-row resulting in a loss
for the player who made that move. As such, `get_result()` is overridden in
`Tictactwo_model`. 

In addition to overriding those methods, the model should also keep track of any
other state that captures the current board state. For example, the tictactwo
model keeps track of board state using the built in `Board2D` class and the
origin of the moveable grid using the built in `Point2D` class. Note that
`Board2D` which is just an extension of `Game_object` is just one way to
encapsulate board state. For example, for a card game, you would extend
`Game_object` and also create a serializer for it extending
`Model_to_strings_mapper`.

Finally, the model should keep track of any `Rules` it uses in determining game
over and/or the game result. A rule simply checks whether a particular game
condition is met. For example, in tictactwo `Rule_board_is_full` checks to see
if the board has been completely filled.

```c++
  bool Rule_board_is_full::check_board_is_full(Tictactwo_model* model) {
    bool result = true;
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        if (model->get_element({i, j}) == nullptr)
          return false;
      }
    }
    return result;
  }
```

This rule along with any rules that determine a win condition for a player are
utilized in the model to determine if the game is over.

```c++
  bool Tictactwo_model::check_win_condition() {
    bool result = has_alignment.check(this);
    winner = has_alignment.get_causal_player();
    return result;
  }

```
