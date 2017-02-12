add_subdirectory(serverstate)
add_subdirectory(database)
add_subdirectory(gamelogic)
add_subdirectory(networking)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME}
  serverstate
  database
  gamelogic
  networking
  )
