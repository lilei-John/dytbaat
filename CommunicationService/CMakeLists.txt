add_library(CommunicationService CommunicationService.h CommunicationService.cpp Logger/Logger.cpp Logger/Logger.h Logger/FsWriter.h ../Applications/Messenger.cpp)

add_subdirectory(Media)
add_subdirectory(TransmissionLayer)
add_subdirectory(DataLinkLayer)
add_subdirectory(Logger)

target_link_libraries(CommunicationService Media TransmissionLayer DataLinkLayer Logger)

add_executable(FrameTravelTimeTest Test/FrameTravelTimeTest.cpp)
target_link_libraries(FrameTravelTimeTest CommunicationService StopAndWait AcousticTL RealAudio)

add_executable(transmitAndOnReceiveCallback Test/transmitAndOnReceiveCallback.cpp)
target_link_libraries(transmitAndOnReceiveCallback CommunicationService StopAndWait AcousticTL IdealAudio)

add_executable(SelectiveRepeatTest Test/SelectiveRepeatTest.cpp)
target_link_libraries(SelectiveRepeatTest CommunicationService SelectiveRepeat AcousticTL RealAudio)