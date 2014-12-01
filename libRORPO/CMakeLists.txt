# RORPO Lib

project(libRORPO)
cmake_minimum_required(VERSION 2.8)

include_directories(
	${PROJECT_SOURCE_DIR}/Pink)

set( SOURCES_RORPO_FILES
     ${PROJECT_SOURCE_DIR}/Pink/lgeodesic.c
     ${PROJECT_SOURCE_DIR}/Pink/mclifo.c
     ${PROJECT_SOURCE_DIR}/Pink/mcfifo.c
	 ${PROJECT_SOURCE_DIR}/Pink/mcindic.c
     ${PROJECT_SOURCE_DIR}/Pink/mccodimage.c
     ${PROJECT_SOURCE_DIR}/Pink/mcimage.c)	

add_library(RORPO SHARED ${SOURCES_RORPO_FILES})

