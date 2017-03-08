add_library(gamelogic_games INTERFACE)

add_subdirectory(tictactoe)

target_link_libraries(gamelogic_games INTERFACE
  gamelogic_games_tictactoe)
