add_library(ori_kin
  edge.cpp
  node.cpp
  structure.cpp
  jsonParser.cpp
  body.cpp
  linAlg.cpp
  constraints.cpp
  targetVelocity.cpp
)



target_include_directories(ori_kin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:include>
)

if(LAPACK_FOUND AND BLAS_FOUND AND LAPACKE_FOUND)
  set(lapackblas_libraries ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${LAPACKE_LIBRARIES})
  target_link_libraries(ori_kin ${lapackblas_libraries})
endif()