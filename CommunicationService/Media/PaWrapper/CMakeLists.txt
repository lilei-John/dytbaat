add_library(PaWrapper PaWrapper.h PaWrapper.cpp PaLifeHandler.h PaCallBackData.h)

target_link_libraries(PaWrapper ${PORTAUDIO_LIBRARIES})

add_executable(PAWOutputTest Test/Sine.cpp)
target_link_libraries(PAWOutputTest PaWrapper)

add_executable(PAWInputTest Test/Mic.cpp)
target_link_libraries(PAWInputTest PaWrapper)

add_executable(PAWMultiInstanceTest Test/MultipleInstances.cpp)
target_link_libraries(PAWMultiInstanceTest PaWrapper)