# RORPO Lib

project(RORPO)
cmake_minimum_required(VERSION 2.8)

set(LIBRARY_OUTPUT_PATH /medit/data/lib/RORPO)

include_directories(
	${PROJECT_SOURCE_DIR}/Pink)
	

set( SOURCES_RORPO_FILES
     ${PROJECT_SOURCE_DIR}/Pink/larith.c
     ${PROJECT_SOURCE_DIR}/Pink/lgeodesic.c
     ${PROJECT_SOURCE_DIR}/Pink/mclifo.c
     ${PROJECT_SOURCE_DIR}/Pink/mcfifo.c
	 ${PROJECT_SOURCE_DIR}/Pink/mcindic.c
     ${PROJECT_SOURCE_DIR}/Pink/mccodimage.c
     ${PROJECT_SOURCE_DIR}/Pink/mcimage.c)

set( HEADER_RORPO_FILES
	${PROJECT_SOURCE_DIR}/Pink/mcutil.h
	${PROJECT_SOURCE_DIR}/min_modif.hpp
    ${PROJECT_SOURCE_DIR}/max_modif.hpp
    ${PROJECT_SOURCE_DIR}/geodilat_modif.hpp
    ${PROJECT_SOURCE_DIR}/diff_buffer.hpp
    ${PROJECT_SOURCE_DIR}/sorting.hpp
    ${PROJECT_SOURCE_DIR}/PO.hpp
    ${PROJECT_SOURCE_DIR}/RORPO.hpp
    ${PROJECT_SOURCE_DIR}/Get_RPO_Orientations.hpp
    ${PROJECT_SOURCE_DIR}/RORPO_multiscale.hpp
    ${PROJECT_SOURCE_DIR}/Pink/larith.h
    ${PROJECT_SOURCE_DIR}/Pink/liarp.h
	${PROJECT_SOURCE_DIR}Pink/generic_macros.h
	${PROJECT_SOURCE_DIR}/Pink/mccodimage.h
	${PROJECT_SOURCE_DIR}/Pink/mcimage.h
	${PROJECT_SOURCE_DIR}/Pink/mcutil.h
	${PROJECT_SOURCE_DIR}/Pink/mcindic.h
    ${PROJECT_SOURCE_DIR}/Pink/mclifo.h
    ${PROJECT_SOURCE_DIR}/Pink/mcfifo.h
	${PROJECT_SOURCE_DIR}/Pink/lgeodesic.h
	${PROJECT_SOURCE_DIR}/genfmax.hpp
	${PROJECT_SOURCE_DIR}/genfmin.hpp
	${PROJECT_SOURCE_DIR}/Pink/rect3dmm.hpp)	

add_library(RORPO SHARED ${SOURCES_RORPO_FILES})

