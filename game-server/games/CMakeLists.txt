add_library(games INTERFACE)

add_subdirectory(tictactoe)
add_subdirectory(tictactwo)

target_link_libraries(games INTERFACE
  games_tictactoe
  games_tictactwo
  )
