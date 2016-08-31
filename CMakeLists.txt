cmake_minimum_required(VERSION 2.8.4)
project(mini3d)

add_definitions(-std=c++11 -Wall)
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS "-g -O2")

include_directories("./include")

file(GLOB MINI3D_DEPENDS "src/*.cc")
file(GLOB MINI3D_MAIN_DEPENDS "mini3d/main.cc")

add_executable(mini3d ${MINI3D_DEPENDS} ${MINI3D_MAIN_DEPENDS}) 

find_package(Curses REQUIRED)
find_library(pthread pthread)

target_link_libraries(mini3d ${CURSES_LIBRARIES})

file(GLOB MINI3D_TEST_DEPENDS "test/test.cc")

include_directories("./deps/googletest/googletest")
include_directories("./deps/googletest/googletest/include")

file(GLOB GTEST_DEPENDS
  "./deps/googletest/googletest/src/gtest-all.cc"
  "./deps/googletest/googletest/src/gtest_main.cc"
  )

add_executable(mini3d_test
  ${MINI3D_DEPENDS}
  ${MINI3D_TEST_DEPENDS}
  ${GTEST_DEPENDS}
  )

target_link_libraries(mini3d_test ${CURSES_LIBRARIES} ${pthread})

add_custom_command(TARGET mini3d_test
  POST_BUILD COMMAND mini3d_test
  )

