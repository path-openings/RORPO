project(Nifti)
cmake_minimum_required(VERSION 2.8)

set(NIFTI_SOURCES 
        ${PROJECT_SOURCE_DIR}/src/nifti1_io.c
	${PROJECT_SOURCE_DIR}/src/znzlib.c)

set( NIFTI_HEADERS
        ${PROJECT_SOURCE_DIR}/include/nifti/nifti1.h
        ${PROJECT_SOURCE_DIR}/include/nifti/nifti1_io.h
	${PROJECT_SOURCE_DIR}/include/nifti/znzlib.h)

add_library(Nifti SHARED ${NIFTI_SOURCES})

install( FILES ${NIFTI_HEADERS} DESTINATION include/nifti)
install( TARGETS Nifti DESTINATION lib)

