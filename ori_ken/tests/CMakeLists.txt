<<<<<<< HEAD
add_executable(verification verification_tests.cpp)
target_link_libraries(verification PRIVATE ori_ken)
target_include_directories(verification PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ori_ken/include>
  $<INSTALL_INTERFACE:include>
)

find_package(LAPACK)
if(LAPACK_FOUND AND BLAS_FOUND)
   set(lapackblas_libraries ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
   target_link_libraries(verification ${lapackblas_libraries})
endif()
=======
# include(add_spmv_test)
# include(add_spmv_test_fail)

# add_spmv_test(./example_unit_test.cpp)
# add_spmv_test_fail(./example_unit_test_fail.cpp)

# add_spmv_test(./testCOO_Constructor.cpp)
>>>>>>> origin/meshing
