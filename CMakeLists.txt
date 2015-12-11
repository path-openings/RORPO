project(RORPO)
cmake_minimum_required(VERSION 2.8)

# C++ 11 activation flag
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++1y")
endif()

# Build Type
IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Release
       CACHE STRING "Choose the type of build : Debug or Release."
       FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
message("Build Type: " ${CMAKE_BUILD_TYPE})


include_directories(
            libRORPO/include
            Image/include
            Image/Nifti/include
            docopt.cpp)

add_subdirectory(Image)
add_subdirectory(libRORPO)
add_subdirectory(docopt.cpp)
add_subdirectory(RPO_usage)
add_subdirectory(RPO_multiscale)
add_subdirectory(RORPO_multiscale_Usage)
