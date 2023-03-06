cmake_minimum_required(VERSION 3.0)

project(MYSORT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

add_executable(mysort_exe mysort_main.cpp src/mysort.cpp)