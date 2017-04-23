add_library(database
  firebase.cc
  )

target_link_libraries(database
  ${CURL_LIBRARIES})
