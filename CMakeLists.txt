cmake_minimum_required (VERSION 3.9)                  # CMake minimum version
project(spef_parse)                                          # your OpenTimer application

set(CMAKE_CXX_STANDARD 17)                            # enable c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "./boost_1_67_0")
include_directories(${INCLUDE_DIRECTORIES} ${BOOST_ROOT} include)

add_executable(spef_parse main.cpp)                           # executable and linkage
