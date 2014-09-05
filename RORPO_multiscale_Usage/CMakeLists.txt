# RORPO_multiscale executable 

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

find_package(VTK REQUIRED)
if(VTK_FOUND)
message("VTK FOUND")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_VTK_H")
include(${VTK_USE_FILE})
endif()
	
FIND_LIBRARY(NIFTI_LIB Nifti /medit/data/lib/Nifti)
include_directories (/medit/data/lib/Nifti/src) 

FIND_LIBRARY(RORPO_LIB RORPO /medit/data/lib/RORPO)
include_directories (/medit/data/lib/RORPO/header) 

set( SOURCES_FILES
	${PROJECT_SOURCE_DIR}/main.cpp)

add_executable( ${PROJECT_NAME} ${SOURCES_FILES} ${PROJECT_SOURCE_DIR} )

target_link_libraries( ${PROJECT_NAME} ${NIFTI_LIB} ${VTK_LIBRARIES} ${RORPO_LIB} )


