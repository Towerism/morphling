add_subdirectory(serverstate)
add_subdirectory(database)
add_subdirectory(gamelogic)
add_subdirectory(networking)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME}
  gamelogic
  networking
  database
  serverstate
  gamelogic_games
  ${THIRD_PARTY_LIBS}
  )
