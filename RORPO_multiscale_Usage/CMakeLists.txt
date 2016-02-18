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

# ADD FILES
file(GLOB_RECURSE HEADER_FILES *.hpp *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable( ${PROJECT_NAME} ${SOURCE_FILES})

# LINK REQUIRED LIBS
target_link_libraries( ${PROJECT_NAME} Nifti RORPO docopt )

install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin/ )



