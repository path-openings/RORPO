project(Image)
cmake_minimum_required(VERSION 3.10)

add_subdirectory(Nifti)

# ADD FILES
file(GLOB_RECURSE Image_HEADERS include/*.hpp include/*.h)

install( FILES ${Image_HEADERS} DESTINATION include/Image)
