cmake_minimum_required(VERSION 3.6)
project(dytbaat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Portaudio REQUIRED)

set(SOURCE_FILES main.cpp CommunicationService.cpp CommunicationService.h DataLinkLayer.h TransmissionLayer.h CommunicationService.cpp TransmissionLayer.cpp DataLinkLayer.cpp)

include_directories(${PORTAUDIO_INCLUDE_DIR})

set(LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})

add_executable(dytbaat ${SOURCE_FILES})
target_link_libraries(dytbaat ${LIBS})