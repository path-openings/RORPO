project(Nifti)
cmake_minimum_required(VERSION 2.8)

include_directories(
	${PROJECT_SOURCE_DIR}/src/)

set( SOURCES_RORPO_FILES
     ${PROJECT_SOURCE_DIR}/src/nifti1_io.c
     ${PROJECT_SOURCE_DIR}/src/znzlib.c)

set( HEADER_RORPO_FILES
	${PROJECT_SOURCE_DIR}/src/nifti1.h
	${PROJECT_SOURCE_DIR}/src/nifti1_io.h
	${PROJECT_SOURCE_DIR}/src/znzlib.h)	

set(NIFTI_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE FILEPATH "Directory to libNifti headers" )

add_library(Nifti SHARED ${SOURCES_RORPO_FILES})

