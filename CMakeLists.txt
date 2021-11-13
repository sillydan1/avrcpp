cmake_minimum_required(VERSION 3.7)
project(avrcpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11) # 20 is a cmake 3.21+ feature
# Remove the implicit link libraries and directories
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")

include_directories(
        include
)

add_library(avrcpp
        src/utillities.cpp
)

# This is only used internally to make my clangd-tidy happy. Dont link to this
add_library(__avrcpp_is src/avrcpp_includer.cpp)
