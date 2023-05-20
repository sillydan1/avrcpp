cmake_minimum_required(VERSION 3.7)
project(avrcpp VERSION 1.1.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11) # 20 is a cmake 3.21+ feature
message("${CMAKE_PROJECT_NAME} version ${CMAKE_PROJECT_VERSION}")
option(DISABLE_TESTS "Disable inclusion of the unit tests (useful if you dont want to depend on GTEST)" OFF)

add_library(avrcpp src/utillities.cpp)

# This is only used internally to make my clangd-tidy happy. Dont link to this
add_library(__avrcpp_is src/avrcpp_includer.cpp)
target_compile_options(__avrcpp_is PUBLIC -nodefaultlibs)
target_link_libraries(__avrcpp_is avrcpp)

if(NOT DISABLE_TESTS)
        add_subdirectory( test )
endif()

