project(RORPO)
cmake_minimum_required(VERSION 2.8)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++1y") # C++ 11 activation flag
	SET(LIB_TYPE SHARED)
else()
	SET(LIB_TYPE STATIC)
endif()

# Build Type
if(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Release
       CACHE STRING "Choose the type of build : Debug or Release."
       FORCE)
endif()
message("Build Type: " ${CMAKE_BUILD_TYPE})

include_directories(
            libRORPO/include
            Image/include
            Image/Nifti/include
            docopt)

add_subdirectory(Image)
add_subdirectory(libRORPO)
add_subdirectory(docopt)
add_subdirectory(RORPO_multiscale_Usage)
