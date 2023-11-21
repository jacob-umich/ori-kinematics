add_executable(cube create_cube.cpp)

target_include_directories(cube PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ori_ken/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(cube ori_ken)