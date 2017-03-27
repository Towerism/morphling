include(gtest)

set(GTEST_LIBS
  gtest
  gmock
  gmock_main
  )

set(TEST_LIBS
  gamelogic
  networking
  database
  serverstate
  gamelogic_games
  ${GTEST_LIBS}
  ${THIRD_PARTY_LIBS}
  )

if (${TEST})
  add_subdirectory(unit)
  add_executable(morphling-test-unit
    $<TARGET_OBJECTS:unit>
    $<TARGET_OBJECTS:tictactwo_tests>)
  target_link_libraries(morphling-test-unit ${TEST_LIBS})
endif()
