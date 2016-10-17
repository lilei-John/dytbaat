add_library(AcousticTL AcousticTL.h AcousticTL.cpp helperFunctions.h)

target_link_libraries(AcousticTL TransmissionLayer)

add_subdirectory(FrameProtocol)
add_subdirectory(Sync)
add_subdirectory(FrameReceiver)
add_subdirectory(DtmfSpec)
add_subdirectory(FreqGeneration)
add_subdirectory(DtmfAnalysis)
add_subdirectory(PaWrapper)

target_link_libraries(AcousticTL FrameProtocol Sync FrameReceiver DtmfSpec FreqGeneration DtmfAnalysis PaWrapper)

add_executable(atlPlayFrame Test/atlPlayFrame.cpp)
target_link_libraries(atlPlayFrame AcousticTL)
