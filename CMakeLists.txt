cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall")

project(matrix_test)

include_directories("${PROJECT_SOURCE_DIR}/src/include")

add_executable(matrix_test "src/test.cpp")