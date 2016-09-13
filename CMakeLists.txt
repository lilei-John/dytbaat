cmake_minimum_required(VERSION 3.6)
project(dytbaat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Dog.cpp Dog.h Cat.cpp Cat.h)
add_executable(dytbaat ${SOURCE_FILES})