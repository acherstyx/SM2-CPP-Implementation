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
set(INNER_HEADER_SOURCE src/sm2/inner_utils.h src/sm3/sm3_miracl_wrapper.h src/sm3/sm3_reference.h)
set(MAIN_SOURCE src/sm2/inner_utils.cpp src/sm3/sm3_miracl_wrapper.cpp src/sm3/sm3_reference.cpp)

# executable target
add_executable(TestMiracl test/test_miracl_env.cpp)
target_link_libraries(TestMiracl ${LIBMIRACL} LIBBIG)

add_executable(SM3UnitTest test/sm3/test_sm3.cpp ${HEADER_SOURCE} ${MAIN_SOURCE} miracl/big.cpp)
target_link_libraries(SM3UnitTest ${LIBMIRACL} LIBBIG)

add_executable(SM2UnitTest test/sm2/test_sm2.cpp ${HEADER_SOURCE} ${MAIN_SOURCE} miracl/big.cpp)
target_link_libraries(SM2UnitTest ${LIBMIRACL} LIBBIG)