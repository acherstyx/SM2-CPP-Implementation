cmake_minimum_required(VERSION 3.13)  # CMake version check
project(simple_example)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++14 standard
add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -O2 ")

#include
include_directories(include)

include_directories(src)


# miracl
include_directories(miracl)
add_library(LIBMIRACL miracl/miracl.a)
set_target_properties(LIBMIRACL PROPERTIES LINKER_LANGUAGE CXX)
add_library(LIBBIG miracl/big.cpp miracl/big.h)

set(HEADER_SOURCE include/sm2.h)
set(MAIN_SOURCE src/sm2/utils.cpp)
set(TEST_SOURCE test/sm2/test_utils.cpp)

# executable target
add_executable(TestMiracl test/test_miracl_env.cpp)
target_link_libraries(TestMiracl ${LIBMIRACL} LIBBIG)

add_executable(UnitTest ${TEST_SOURCE} ${HEADER_SOURCE} ${MAIN_SOURCE} miracl/big.cpp)
target_link_libraries(UnitTest ${LIBMIRACL} LIBBIG)
