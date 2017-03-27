add_library(gamelogic_games INTERFACE)

add_subdirectory(tictactoe)
add_subdirectory(tictactwo)

target_link_libraries(gamelogic_games INTERFACE
  gamelogic_games_tictactoe
  gamelogic_games_tictactwo
  )
