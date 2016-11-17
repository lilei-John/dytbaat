cmake_minimum_required(VERSION 3.6)
project(dytbaat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/runtime/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/runtime/)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(Portaudio REQUIRED)
include_directories(${PORTAUDIO_INCLUDE_DIR})

add_subdirectory(PaWrapper)
add_subdirectory(CommunicationService)
add_subdirectory(Applications)