cmake_minimum_required(VERSION 3.0.0)
project(mini3d VERSION 0.0.0)

add_definitions(-std=c++11 -Wall -O2)

include_directories("./src/")

file(GLOB MINI3D_DEPENDS "src/*.cpp")
add_executable(mini3d ${MINI3D_DEPENDS})

find_package(Curses REQUIRED)
target_link_libraries(mini3d ${CURSES_LIBRARIES})
