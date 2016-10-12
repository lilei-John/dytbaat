set(ATLSources AcousticTL.h AcousticTL.cpp)
add_library(AucousticTL ${ATLSources})

add_subdirectory(PaWrapper)
add_subdirectory(Sync)
add_subdirectory(FrameReceiver)

target_link_libraries(AucousticTL PaWrapper Sync FrameReceiver)