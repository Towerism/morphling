include(gtest)

set(GTEST_LIBS
  gtest
  gtest_main
  )

set(TEST_LIBS
  gamelogic
  networking
  database
  serverstate
  ${GTEST_LIBS}
  )

if (${TEST})
  add_subdirectory(unit)
  add_executable(morphling-test-unit $<TARGET_OBJECTS:unit>)
  target_link_libraries(morphling-test-unit ${TEST_LIBS})
endif()
