cmake_minimum_required(VERSION 3.0)
find_package(GTest)
if(${GTEST_FOUND})
    enable_testing()
    # Enable implementation peeking
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAVRCPP_DEBUG")
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(unittests main.cpp)
    target_link_libraries(unittests ${GTEST_LIBRARIES})
endif()
