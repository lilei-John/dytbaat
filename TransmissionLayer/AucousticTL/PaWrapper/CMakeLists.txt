set(PAWSources PaWrapper.h PaWrapper.cpp PaLifeHandler.h PaCallBackData.h)
add_library(PaWrapper ${PAWSources})

#portaudio
find_package(Portaudio REQUIRED)
include_directories(${PORTAUDIO_INCLUDE_DIR})
target_link_libraries(PaWrapper ${PORTAUDIO_LIBRARIES})

add_executable(PAWOutputTest Test/Sine.cpp)
target_link_libraries(PAWOutputTest PaWrapper)

add_executable(PAWInputTest Test/Mic.cpp)
target_link_libraries(PAWOutputTest PaWrapper)