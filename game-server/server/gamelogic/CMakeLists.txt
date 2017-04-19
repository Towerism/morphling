add_library(gamelogic INTERFACE)

add_subdirectory(controller)
add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(games)

target_link_libraries(gamelogic INTERFACE
  gamelogic_controller
  gamelogic_model
  gamelogic_view
  )
