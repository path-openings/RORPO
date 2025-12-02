cmake_minimum_required(VERSION 3.10)
project(RORPO VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(UNIX)
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

set(PYTHON_BINDING ON CACHE BOOL "enable python binding")
set(3DSLICER_BINDING ON CACHE BOOL "enable 3DSlicer module")

include_directories(
	    libRORPO/include
	    Image/include
	    docopt)

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
	enable_testing()
endif()

add_subdirectory(libRORPO)

if(NOT 3DSLICER_BINDING)
add_subdirectory(docopt)
endif()

add_subdirectory(RORPO_multiscale_Usage)

# retrieving pybind11
if(PYTHON_BINDING)
	include_directories(pyRORPO)
	add_subdirectory(pyRORPO)
endif()

