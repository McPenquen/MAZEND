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

#### RESOURCES FILE ####
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

#### EXTERNAL DEPENDENCIES ####
# SFML
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# BOX2D
add_subdirectory("lib/b2d/Box2D")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### LIBRARIES ####
# ENGINE
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_tile_level_system lib_maths lib_ecm Box2D sfml-graphics sfml-audio)

# Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

# ECM lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_ecm/*.cpp engine/lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths lib_tile_level_system)

## Tile map lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_tile_level_system/*.cpp engine/lib_tile_level_system/*.h)
add_library(lib_tile_level_system STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_system INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_tile_level_system/" )
target_link_libraries(lib_tile_level_system lib_maths sfml-graphics)

#### THE MAIN MAZEND GAME VS PROJECT ####
file(GLOB_RECURSE SOURCES mazend_game/*.cpp mazend_game/*.h)
file(GLOB_RECURSE CMPNTS mazend_game/components/*.cpp mazend_game/components/*.h)
file(GLOB_RECURSE SCENES mazend_game/scenes/*.cpp mazend_game/scenes/*.h)
add_executable(MAZEND_GAME ${SOURCES} ${CMPNTS} ${SCENES})
source_group("components" FILES ${CMPNTS})
source_group("scenes" FILES ${SCENES})
target_link_libraries(MAZEND_GAME lib_engine)
set(EXECUTABLES ${EXECUTABLES} MAZEND_GAME)
set_target_properties(MAZEND_GAME 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
add_dependencies(MAZEND_GAME copy_resources)
