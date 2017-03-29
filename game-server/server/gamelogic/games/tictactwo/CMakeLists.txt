add_library(gamelogic_games_tictactwo
  tictactwo_model.cc
  action_move_grid.cc
  action_place_piece.cc
  action_move_piece.cc
  rule_board_is_full.cc
  )

target_link_libraries(gamelogic_games_tictactwo
  gamelogic
  )
