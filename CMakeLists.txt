cmake_minimum_required(VERSION 3.0.0)
project(mini3d VERSION 0.0.0)

set(CMAKE_CXX_FLAGS "${CMKAE_CXX_FLAGS} -std=c++11 -lncurses")

file(GLOB MINI3D_DEPENDS "src/*.cpp" "src/*.h")
add_executable(mini3d ${MINI3D_DEPENDS})