add_executable(verification verification_tests.cpp)
add_executable(st simple_test.cpp)
add_executable(json jsonTests.cpp)

configure_file(jsonTest.json ${CMAKE_CURRENT_BINARY_DIR}/jsonTest.json COPYONLY)

find_package(LAPACK)
if(LAPACK_FOUND AND BLAS_FOUND)
  set(lapackblas_libraries ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
  target_link_libraries(verification ${lapackblas_libraries} ori_kin)
else()
  target_link_libraries(verification PRIVATE ori_kin)
  target_include_directories(verification PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
  )
  
  endif()
  target_link_libraries(st PRIVATE ori_kin)
  target_link_libraries(json PRIVATE ori_kin)
  target_include_directories(st PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
  )
  target_include_directories(json PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
  )

add_test(verification verification)
add_test(st st)
add_test(json json)
