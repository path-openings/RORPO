# RORPO_multiscale usage executable
cmake_minimum_required(VERSION 3.10)
project (RORPO_multiscale_usage)


# FIND OPENMP
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OpenMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

# Find ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if(3DSLICER_BINDING)
    add_definitions(-DSLICER_BINDING)
    message("Generating 3D Slicer binding")
    # GenerateCLP
    find_package(SlicerExecutionModel REQUIRED)
    include(${SlicerExecutionModel_USE_FILE})
    # Slicer macro
    SEMMacroBuildCLI(
       NAME RORPO_multiscale_usage
       TARGET_LIBRARIES RORPO ${ITK_LIBRARIES}
       INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
   )
else()
   add_executable( ${PROJECT_NAME} RORPO_multiscale_usage.cxx )
   # LINK REQUIRED LIBS
   target_link_libraries( ${PROJECT_NAME} RORPO docopt ${ITK_LIBRARIES} )
   set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
   install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin/ )
endif()

add_subdirectory(tests)
