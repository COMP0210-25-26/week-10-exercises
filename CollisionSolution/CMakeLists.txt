cmake_minimum_required(VERSION 3.16)
project(hello)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3")

option(GRAPHICS "Enable graphics" OFF)

if(GRAPHICS)
    add_definitions(-DGRAPHICS)
    add_subdirectory(SDL EXCLUDE_FROM_ALL)
endif()

find_package(MPI REQUIRED)

add_subdirectory(source)