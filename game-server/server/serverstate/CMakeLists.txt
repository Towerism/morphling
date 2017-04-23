add_library(serverstate
  server_state.cc
  game_instance.cc
  )

target_link_libraries(serverstate
  database
  gamelogic
  )
