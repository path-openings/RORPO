# RORPO_multiscale usage executable 

project (RORPO_multiscale)
cmake_minimum_required(VERSION 2.8)

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/Pink)
	
	
# FIND OPENMP
FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

endif()
	
FIND_LIBRARY(NIFTI_LIB Nifti $ENV{NIFTI_LIB_PATH})

set(NIFTI_HEADER_DIR  "" CACHE FILEPATH "Directory to libNifti headers" )
if (NOT EXISTS "${NIFTI_HEADER_DIR}/nifti1.h")
    message(FATAL_ERROR "Can't find libNifti headers in NIFTI_HEADER_DIR")
endif()

include_directories (${NIFTI_HEADER_DIR}) 

FIND_LIBRARY(RORPO_LIB RORPO $ENV{RORPO_LIB_PATH})

set(RORPO_HEADER_DIR  "" CACHE FILEPATH "Directory to libRORPO headers" )
if (NOT EXISTS "${RORPO_HEADER_DIR}/Image.hpp")
    message(FATAL_ERROR "Can't find libRORPO headers in RORPO_HEADER_DIR")
endif()
include_directories (${RORPO_HEADER_DIR}) 
include_directories (${RORPO_HEADER_DIR}/Pink)

set( SOURCES_FILES
	${PROJECT_SOURCE_DIR}/main.cpp)

add_executable( ${PROJECT_NAME} ${SOURCES_FILES} ${PROJECT_SOURCE_DIR} )

if(NIFTI_LIB AND RORPO_LIB)
    target_link_libraries( ${PROJECT_NAME} ${NIFTI_LIB} ${VTK_LIBRARIES} ${RORPO_LIB} )
else()
    target_link_libraries( ${PROJECT_NAME} Nifti ${VTK_LIBRARIES} RORPO)
endif()


