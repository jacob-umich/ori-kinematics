add_library(ori_kin
  edge.cpp
  node.cpp
  structure.cpp
  jsonParser.cpp
  body.cpp
  linAlg.cpp
  constraints.cpp
)



target_include_directories(ori_kin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:include>
)
