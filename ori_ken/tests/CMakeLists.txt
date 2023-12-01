add_executable(verification verification_tests.cpp)
add_executable(st simple_test.cpp)

find_package(LAPACK)
if(LAPACK_FOUND AND BLAS_FOUND)
  set(lapackblas_libraries ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
  target_link_libraries(verification ${lapackblas_libraries} ori_ken)
else()
  target_link_libraries(verification PRIVATE ori_ken)
  target_include_directories(verification PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
  )
  
  endif()
  target_link_libraries(st PRIVATE ori_ken)
  target_include_directories(st PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
  )

add_test(verification verification)
add_test(st st)
