cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

project(Tracing)

option(BUILD_TRAYC "Build the Trayc library" OFF)
option(BUILD_FUNCTIONDRAWER "Build the FunctionDrawer" ON)
option(BUILD_GAMEBLOCKS "Build GameBlocks" ON)
option(BUILD_BALLPHYSICS "Build BallPhysics" ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(SDL2_BUILDING_LIBRARY FALSE)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR} 
    ${GLEW_INCLUDE_PATH}
    ${OPENGL_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/HeaderLibs"
)

add_subdirectory(Engine)

if(BUILD_TRAYC)
    add_subdirectory(Trayc)
    add_subdirectory(Tester)
endif(BUILD_TRAYC)

if(BUILD_FUNCTIONDRAWER)
    add_subdirectory(FunctionDrawer)
endif(BUILD_FUNCTIONDRAWER)

if(BUILD_GAMEBLOCKS)
    add_subdirectory(GameBlocks)
endif(BUILD_GAMEBLOCKS)

if(BUILD_BALLPHYSICS)
    add_subdirectory(BallPhysics)
endif(BUILD_BALLPHYSICS)
