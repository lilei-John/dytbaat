cmake_minimum_required(VERSION 3.6)
project(dytbaat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Portaudio REQUIRED)

set(SOURCE_FILES CommunicationService.cpp CommunicationService.h DataLinkLayer.h TransmissionLayer/TransmissionLayer.h CommunicationService.cpp TransmissionLayer/TransmissionLayer.cpp DataLinkLayer.cpp TransmissionLayer/AucousticTL/AucousticTL.cpp TransmissionLayer/AucousticTL/AucousticTL.h TransmissionLayer/AucousticTL/PaWrapper/PaLifeHandler.h TransmissionLayer/AucousticTL/PaWrapper/PaWrapper.cpp TransmissionLayer/AucousticTL/PaWrapper/PaWrapper.h)

include_directories(${PORTAUDIO_INCLUDE_DIR})

set(LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})

add_executable(main main.cpp ${SOURCE_FILES})
add_executable(sine TransmissionLayer/AucousticTL/PaWrapper/Test/Sine.cpp ${SOURCE_FILES})
target_link_libraries(main ${LIBS})
target_link_libraries(sine ${LIBS})