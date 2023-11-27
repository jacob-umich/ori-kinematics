add_executable(cube create_cube.cpp)
configure_file(cube.json ${CMAKE_CURRENT_BINARY_DIR}/cube.json COPYONLY)

target_include_directories(cube PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ori_ken/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(cube ori_ken)