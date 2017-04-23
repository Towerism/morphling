add_library(networking
  gcp_socket.cc
  gcp_client.cc
  gcp_server.cc
  gcp_server_socket.cc
  )

target_link_libraries(networking
  gamelogic
  serverstate
  )
