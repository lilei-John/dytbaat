add_library(IdealAudio IdealAudio.h IdealAudio.cpp)
target_link_libraries(IdealAudio Media)

add_executable(idealAudioTest Test/idealAudioTest.cpp)
target_link_libraries(idealAudioTest IdealAudio)