add_library(AucousticTL AcousticTL.h AcousticTL.cpp)

add_subdirectory(PaWrapper)
add_subdirectory(Sync)
add_subdirectory(FrameReceiver)
add_subdirectory(DtmfSpec)

target_link_libraries(AucousticTL PaWrapper Sync FrameReceiver DtmfSpec)
