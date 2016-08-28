cmake_minimum_required(VERSION 2.8.4)
project(mini3d)

add_definitions(-std=c++11 -Wall -g)
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
set(CMAKE_CXX_FLAGS "-O2")

include_directories("./src/")

file(GLOB MINI3D_DEPENDS "src/*.cc")
add_executable(mini3d ${MINI3D_DEPENDS})

find_package(Curses REQUIRED)
target_link_libraries(mini3d ${CURSES_LIBRARIES})
