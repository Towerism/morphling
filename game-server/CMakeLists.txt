cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(morphling-game-server)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(.)

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
