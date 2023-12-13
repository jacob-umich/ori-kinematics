add_executable(cube create_cube.cpp)
add_executable(miura miura.cpp)
configure_file(cube.json ${CMAKE_CURRENT_BINARY_DIR}/cube.json COPYONLY)
configure_file(miura.json ${CMAKE_CURRENT_BINARY_DIR}/miura.json COPYONLY)

target_include_directories(cube PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ori_kin/include>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(miura PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ori_kin/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(cube ori_kin)
target_link_libraries(miura ori_kin)