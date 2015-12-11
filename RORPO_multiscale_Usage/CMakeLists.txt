# RORPO_multiscale usage executable 

project (RORPO_multiscale_usage)
cmake_minimum_required(VERSION 2.8)

# FIND OPENMP
FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OpenMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# FIND libNifti
set(NIFTI_HEADER ${CMAKE_SOURCE_DIR}/Image/Nifti/include CACHE STRING "Directory to libNifti headers")
set(NIFTI_LIB ${CMAKE_BINARY_DIR}/Image/Nifti/libNifti.so CACHE STRING "Path to libNifti.so")


# FIND libRORPO
set(RORPO_HEADER_DIR ${CMAKE_SOURCE_DIR}/libRORPO/include CACHE STRING "Directory to libRORPO headers")
set(RORPO_LIB ${CMAKE_BINARY_DIR}/libRORPO/libRORPO.so CACHE STRING "Path to libRORPO.so")

#option(RORPO_HEADER_DIR "Directory to libRORPO headers" ${CMAKE_SOURCE_DIR}/libRORPO/include)
#option(RORPO_LIB "Directory to libRORPO.so " ${CMAKE_BINARY_DIR}/libRORPO)

set(DOCOPT_LIB ${CMAKE_BINARY_DIR}/docopt.cpp/libdocopt.so CACHE STRING "Path to docopt.so")

# ADD FILES
file(GLOB_RECURSE HEADER_FILES *.hpp *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable( ${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries( ${PROJECT_NAME} ${NIFTI_LIB} ${RORPO_LIB} ${DOCOPT_LIB})

#if(NIFTI_LIB AND RORPO_LIB)
#    target_link_libraries( ${PROJECT_NAME} ${NIFTI_LIB} ${VTK_LIBRARIES} ${RORPO_LIB} )
#else()
#    target_link_libraries( ${PROJECT_NAME} Nifti ${VTK_LIBRARIES} RORPO)
#endif()


