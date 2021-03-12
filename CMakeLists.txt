cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Project name
project(MAZEND)

#### DIRECTORIES SETUP ####
# Main output dir
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# All DLLs from all libs go into the build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### EXTERNAL DEPENDENCIES ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### VS PROJECTS & LIBRARIES ####
## THE MAIN GAME VS PROJECT
file(GLOB_RECURSE SOURCES mazend_game/*.cpp mazend_game/*.h)
add_executable(MAZEND_GAME ${SOURCES})
target_include_directories(MAZEND_GAME PRIVATE ${SFML_INCS})
target_link_libraries(MAZEND_GAME sfml-graphics)
