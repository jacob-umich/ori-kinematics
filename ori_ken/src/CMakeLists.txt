add_library(ori_ken
)

target_include_directories(ori_ken PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:include>
)
