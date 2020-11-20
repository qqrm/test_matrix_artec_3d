cmake_minimum_required (VERSION 3.8)

add_compile_options(/W4 /WX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("matrix_test")

add_executable (matrix_test "matrix_test.cpp" "matrix_test.h" "matrix/matrix.hpp"  )