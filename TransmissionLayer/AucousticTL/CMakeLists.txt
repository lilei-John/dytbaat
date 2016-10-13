add_library(AucousticTL AcousticTL.h AcousticTL.cpp FreqGeneration/FreqGeneration.cpp FreqGeneration/FreqGeneration.h helperFunctions.h)

add_subdirectory(PaWrapper)
add_subdirectory(Sync)
add_subdirectory(FrameReceiver)
add_subdirectory(DtmfSpec)
add_subdirectory(DtmfAnalysis)
add_subdirectory(FreqGeneration)

target_link_libraries(AucousticTL PaWrapper Sync FrameReceiver DtmfSpec DtmfAnalysis FreqGeneration)
