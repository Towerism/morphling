cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(morphling-game-server)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

option(TEST "Enable testing" OFF)

set(CMAKE_CXX_FLAGS "-std=c++1z ${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED)
if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})
  set(THIRD_PARTY_LIBS ${CURL_LIBRARIES})
endif()

set(THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} pthread)

include_directories(server)
add_subdirectory(server)

if (${TEST})
  enable_testing()
  add_subdirectory(test EXCLUDE_FROM_ALL)
  add_custom_target(build-tests)

  add_test(NAME unit COMMAND $<TARGET_FILE:morphling-test-unit>)
  add_dependencies(build-tests morphling-test-unit)
endif()

add_subdirectory(samples)
add_executable(gcp-test-client samples/test_gcp_socket.cc)
target_link_libraries(gcp-test-client
  serverstate
  database
  gamelogic
  networking
  ${THIRD_PARTY_LIBS}
  )

