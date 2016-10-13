add_library(AucousticTL AcousticTL.h AcousticTL.cpp helperFunctions.h)

add_subdirectory(PaWrapper)
add_subdirectory(Sync)
add_subdirectory(FrameReceiver)
add_subdirectory(DtmfSpec)
add_subdirectory(DtmfAnalysis)
add_subdirectory(FreqGeneration)

target_link_libraries(AucousticTL PaWrapper Sync FrameReceiver DtmfSpec DtmfAnalysis FreqGeneration)

add_executable(atlPlayFrame Test/atlPlayFrame.cpp)
target_link_libraries(atlPlayFrame AcousticTL)
