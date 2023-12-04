add_executable(verification verification_tests.cpp)
add_executable(json_ut jsonTests.cpp)
add_executable(meshing_ut meshing_ut.cpp)
add_executable(meshing_verif meshing_verif.cpp)
add_executable(linAlg_ut linAlg_ut.cpp)
add_executable(constraints_ut constraints_ut.cpp)

configure_file(jsonTest.json ${CMAKE_CURRENT_BINARY_DIR}/jsonTest.json COPYONLY)
configure_file(testMesh.json ${CMAKE_CURRENT_BINARY_DIR}/testMesh.json COPYONLY)

target_link_libraries(verification ori_kin)
target_link_libraries(json_ut ori_kin)
target_link_libraries(meshing_ut ori_kin)
target_link_libraries(meshing_verif ori_kin)
target_link_libraries(linAlg_ut ori_kin)
target_link_libraries(constraints_ut ori_kin)
  
target_include_directories(verification PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(json_ut PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(meshing_ut PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(meshing_verif PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(linAlg_ut PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(constraints_ut PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:include>
)

add_test(verification verification)
add_test(json_ut json_ut)
add_test(meshing_ut meshing_ut)
add_test(meshing_verif meshing_verif)
add_test(linAlg_ut linAlg_ut)
add_test(constraints_ut constraints_ut)
