add_library(PaWrapper PaWrapper.h PaWrapper.cpp PaLifeHandler.h PaCallBackData.h)

target_link_libraries(PaWrapper ${PORTAUDIO_LIBRARIES})

add_executable(PaWSine Test/Sine.cpp)
target_link_libraries(PaWSine PaWrapper)

add_executable(PaWMic Test/Mic.cpp)
target_link_libraries(PaWMic PaWrapper)

add_executable(PaWMultiInstance Test/MultipleInstances.cpp)
target_link_libraries(PaWMultiInstance PaWrapper)