add_library(gamelogic_model
  model.cc
  )

add_subdirectory(gameobject)

target_link_libraries(gamelogic_model
  gamelogic_model_gameobject
  )
