cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

project(Tracing)

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "/Ox /Ob2 /Oi /Ot /Oy /GT /GL")
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wno-comment -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
endif(MSVC)

INCLUDE(CheckCXXSourceCompiles) 
CHECK_CXX_SOURCE_COMPILES(
"
#include <memory>

int main() {
    std::shared_ptr<int>();
}
"
ENTITYX_HAVE_CXX11_STDLIB
)


option(BUILD_TRAYC "Build the Trayc library" OFF)
option(BUILD_FUNCTIONDRAWER "Build the FunctionDrawer" OFF)
option(BUILD_GAMEBLOCKS "Build GameBlocks" ON)
option(BUILD_BALLPHYSICS "Build BallPhysics" ON)
option(BUILD_MANDELBROT "Build BallPhysics" ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(SDL2_BUILDING_LIBRARY FALSE)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR} 
    ${GLEW_INCLUDE_PATH}
    ${OPENGL_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}
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

if(BUILD_MANDELBROT)
    add_subdirectory(Mandelbrot)
endif(BUILD_MANDELBROT)
