cmake_minimum_required (VERSION 2.6)
project (Myproject)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Werror -std=c++11" )

add_executable(proj main.cpp)
