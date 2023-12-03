cmake_minimum_required(VERSION 3.20)

project(Okin VERSION 0.1.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Check that the compiler supports the C++17 standard
# We assume you are using GNU compilers
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.0.0")
    message(FATAL_ERROR "GCC version must be at least 8.0.0")
  endif()
else()
  message(WARNING "You are using an unsupported compiler! This might not work.")
endif()

# Enable a long list of warnings to ensure correctness
include(gnu_warning_flags)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GNU_WARNING_FLAGS}")

# Enable testing
include(CTest)
include(findLAPACKE)
enable_testing()
find_package(BLAS)
find_package(LAPACK)
find_package(LAPACKE)
if (NOT (LAPACK_FOUND AND BLAS_FOUND AND LAPACKE_FOUND))

  add_compile_definitions(BLAS_FOUND=0)
else ()
  add_compile_definitions(BLAS_FOUND=1)

endif()

add_subdirectory(ori_kin)
add_subdirectory(example)