add_library(RealAudio RealAudio.h RealAudio.cpp)
target_link_libraries(RealAudio Media PaWrapper)

add_executable(RealAudioSine Test/RealAudio.cpp)
target_link_libraries(RealAudioSine RealAudio)
