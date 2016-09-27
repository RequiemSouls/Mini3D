cmake_minimum_required(VERSION 2.8.4)
project(mini3d)

if(CMAKE_CXX_COMPILER_ID MATCHES "GUN|Clang")
add_definitions(-std=c++11 -Wall)
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS "-g -O2")
endif()

include_directories("./include")

file(GLOB MINI3D_DEPENDS "src/*.cc")
file(GLOB MINI3D_MAIN_DEPENDS "mini3d/main.cc")

add_executable(mini3d ${MINI3D_DEPENDS} ${MINI3D_MAIN_DEPENDS}) 

if(UNIX)
find_package(Curses REQUIRED)
find_library(pthread pthread)

target_link_libraries(mini3d ${CURSES_LIBRARIES})
endif(UNIX)

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

if(UNIX)
target_link_libraries(mini3d_test ${CURSES_LIBRARIES} ${pthread})
endif(UNIX)

add_custom_command(TARGET mini3d_test
  POST_BUILD COMMAND mini3d_test
  )

