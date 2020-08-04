cmake_minimum_required(VERSION 3.13)  # CMake version check
project(simple_example)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++14 standard
add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -O2 ")

# test source file
set(TEST_SOURCE test/sm2/test_seed.cpp)

# miracl
include_directories(miracl)
add_library(LIBMIRACL miracl/miracl.a)
set_target_properties(LIBMIRACL PROPERTIES LINKER_LANGUAGE CXX)
# miracl big
add_library(BIG miracl/big.cpp miracl/big.h)

# executable target
add_executable(UnitTest ${TEST_SOURCE})
target_link_libraries(UnitTest ${LIBMIRACL})

add_executable(RUN src/sm2/seed.cpp miracl/big.cpp)
target_link_libraries(RUN ${LIBMIRACL})
target_link_libraries(RUN BIG)


add_executable(TestMiracl test/miracl_env/test_main.cpp miracl/big.cpp)
target_link_libraries(TestMiracl ${LIBMIRACL})
target_link_libraries(TestMiracl BIG)



