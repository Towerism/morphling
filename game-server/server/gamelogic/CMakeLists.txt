add_library(gamelogic
  stub.cc
  )

include_directories(.)

add_subdirectory(controller)
add_subdirectory(model)

target_link_libraries(gamelogic
  gamelogic_controller
  gamelogic_model)
