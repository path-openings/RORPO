project(Nifti)
cmake_minimum_required(VERSION 2.8)
set(LIBRARY_OUTPUT_PATH /medit/data/lib/Nifti)

include_directories(
	${PROJECT_SOURCE_DIR}/src/)

set( SOURCES_RORPO_FILES
     ${PROJECT_SOURCE_DIR}/src/nifti1_io.c
     ${PROJECT_SOURCE_DIR}/src/znzlib.c)

set( HEADER_RORPO_FILES
	${PROJECT_SOURCE_DIR}/src/nifti1.h
	${PROJECT_SOURCE_DIR}/src/nifti1_io.h
	${PROJECT_SOURCE_DIR}/src/znzlib.h)	

add_library(Nifti SHARED ${SOURCES_RORPO_FILES})

