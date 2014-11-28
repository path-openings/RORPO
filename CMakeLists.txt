# RORPO Lib

project(libRORPO)
cmake_minimum_required(VERSION 2.8)

#set(CMAKE_CXX_FLAGS "-std=c++0x)


set(LIBRARY_OUTPUT_PATH /opt/lib/libRORPO)

include_directories(
	${PROJECT_SOURCE_DIR}/Pink)

FIND_LIBRARY(NIFTI_LIB Nifti /opt/lib/Nifti)
include_directories (/opt/lib/Nifti/src) 
	

set( SOURCES_RORPO_FILES
     ${PROJECT_SOURCE_DIR}/Pink/lgeodesic.c
     ${PROJECT_SOURCE_DIR}/Pink/mclifo.c
     ${PROJECT_SOURCE_DIR}/Pink/mcfifo.c
	 ${PROJECT_SOURCE_DIR}/Pink/mcindic.c
     ${PROJECT_SOURCE_DIR}/Pink/mccodimage.c
     ${PROJECT_SOURCE_DIR}/Pink/mcimage.c)	

add_library(RORPO SHARED ${SOURCES_RORPO_FILES} ${NIFTI_LIB})

