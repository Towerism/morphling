add_subdirectory(tictactwo)

add_library(unit OBJECT
  test_gamelogic_model.cc
  test_tictactoe.cc
  test_firebase.cc
  test_gcp_socket_server.cc
  )


