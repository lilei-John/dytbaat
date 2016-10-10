cmake_minimum_required(VERSION 3.6)
project(dytbaat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(SOURCE_FILES CommunicationService.h CommunicationService.cpp DataLinkLayer.h DataLinkLayer.cpp)
add_subdirectory(TransmissionLayer)

add_executable(dytbaat main.cpp)
target_link_libraries(dytbaat TransmissionLayer)