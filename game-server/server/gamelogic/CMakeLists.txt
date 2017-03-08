add_library(gamelogic INTERFACE)

include_directories(.)

add_subdirectory(controller)
add_subdirectory(model)
add_subdirectory(games)

target_link_libraries(gamelogic INTERFACE
  gamelogic_controller
  gamelogic_model
  gamelogic_games)
