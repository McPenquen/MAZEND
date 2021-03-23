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

#### LIBRARIES ####
# Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

# ECM lib
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

## Tile map lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_system/*.cpp lib_tile_level_system/*.h)
add_library(lib_tile_level_system STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_system INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_system/" )
target_link_libraries(lib_tile_level_system lib_maths sfml-graphics)


#### THE MAIN MAZEND GAME VS PROJECT ####
file(GLOB_RECURSE SOURCES mazend_game/*.cpp mazend_game/*.h)
add_executable(MAZEND_GAME ${SOURCES})
target_include_directories(MAZEND_GAME PRIVATE ${SFML_INCS})
target_link_libraries(MAZEND_GAME sfml-graphics lib_ecm)
