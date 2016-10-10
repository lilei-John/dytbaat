set(PAWSources PaWrapper.h PaWrapper.cpp PaLifeHandler.h PaCallBackData.h)
add_library(PaWrapper ${PAWSources})

#portaudio
find_package(Portaudio REQUIRED)
include_directories(${PORTAUDIO_INCLUDE_DIR})
target_link_libraries(PaWrapper ${PORTAUDIO_LIBRARIES})
