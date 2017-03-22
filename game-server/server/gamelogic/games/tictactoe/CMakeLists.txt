add_library(gamelogic_games_tictactoe
  tictactoe_controller.cc
  tictactoe_engine.cc
  tictactoe_model.cc
  rule_has_alignment.cc
  rule_board_is_full.cc
  action_mark.cc
  )

target_link_libraries(gamelogic_games_tictactoe
  gamelogic
  )
