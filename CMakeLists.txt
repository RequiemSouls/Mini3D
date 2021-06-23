cmake_minimum_required(VERSION 2.8.4)
project(mini3d)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(CMAKE_CXX_COMPILER_ID MATCHES "GUN|Clang")
add_definitions(-std=c++11 -Wall)
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS "-g -O2")
endif()

include_directories("./include")
include_directories("./thirdpart")

file(GLOB MINI3D_DEPENDS "src/*.cc")
file(GLOB MINI3D_MAIN_DEPENDS "mini3d/main.cc")
# include thirdpart source
file(GLOB MINI3D_THIRD_PART_DEPENDS "thirdpart/ui/*.*")

add_executable(mini3d ${MINI3D_DEPENDS} ${MINI3D_MAIN_DEPENDS} ${MINI3D_THIRD_PART_DEPENDS}) 

if(WIN32)
set(SDL2_ROOT_DIR D:/Download/SDL2-2.0.14/)
set(SDL2_INCLUDE_DIR ${SDL2_ROOT_DIR}include)
set(SDL2_LIBRARY_DIR ${SDL2_ROOT_DIR}lib/x86)
add_custom_command(TARGET mini3d POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_LIBRARY_DIR}/SDL2.dll $<TARGET_FILE_DIR:mini3d>)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(mini3d ${SDL2_LIBRARY_DIR}/SDL2.lib ${SDL2_LIBRARY_DIR}/SDL2main.lib)
else()
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(mini3d ${SDL2_LIBRARY})
endif()


if(UNIX)
find_library(pthread pthread)
endif()

file(GLOB MINI3D_TEST_DEPENDS "test/test.cc")

include_directories("./deps/googletest/googletest")
include_directories("./deps/googletest/googletest/include")

file(GLOB GTEST_DEPENDS
  "./deps/googletest/googletest/src/gtest-all.cc"
  "./deps/googletest/googletest/src/gtest_main.cc"
  )

# add_executable(mini3d_test
#   ${MINI3D_DEPENDS}
#   ${MINI3D_TEST_DEPENDS}
#   ${GTEST_DEPENDS}
#   )

# if(UNIX)
# target_link_libraries(mini3d_test ${pthread})
# endif(UNIX)

# add_custom_command(TARGET mini3d_test
#   POST_BUILD COMMAND mini3d_test
#   )
