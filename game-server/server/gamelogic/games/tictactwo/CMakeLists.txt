add_library(gamelogic_games_tictactwo
  tictactwo_model.cc
  action_move_grid.cc
  action_place_piece.cc
  )

target_link_libraries(gamelogic_games_tictactwo
  gamelogic
  )
