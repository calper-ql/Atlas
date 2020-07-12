cmake_minimum_required(VERSION 3.17)
project(atlas)

add_subdirectory(googletest-release-1.10.0)
include_directories(${gtest_SOURCE_DIR}/include ${COMMON_INCLUDES})
 
add_executable(runTests test.cpp atlas.hpp)
target_link_libraries(runTests gtest)

