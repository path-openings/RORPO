# RORPO Lib

project(libRORPO)
cmake_minimum_required(VERSION 2.8)

include_directories(
	${PROJECT_SOURCE_DIR}/Pink)
	
include_directories(
	${PROJECT_SOURCE_DIR})

file (GLOB RORPO_HEADERS *.hpp)

set( SOURCES_RORPO_FILES
     ${PROJECT_SOURCE_DIR}/Pink/lgeodesic.c
     ${PROJECT_SOURCE_DIR}/Pink/mclifo.c
     ${PROJECT_SOURCE_DIR}/Pink/mcfifo.c
     ${PROJECT_SOURCE_DIR}/Pink/mcindic.c
     ${PROJECT_SOURCE_DIR}/Pink/mccodimage.c
     ${PROJECT_SOURCE_DIR}/Pink/mcimage.c)	

set(RORPO_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "Directory to libRORPO headers" )

add_library(RORPO SHARED ${SOURCES_RORPO_FILES} ${RORPO_HEADERS})
