
project(RORPO)
cmake_minimum_required(VERSION 2.8)

message("Build Type: " ${CMAKE_BUILD_TYPE})

add_subdirectory(Nifti)
add_subdirectory(libRORPO)
add_subdirectory(RORPO_multiscale_Usage)
