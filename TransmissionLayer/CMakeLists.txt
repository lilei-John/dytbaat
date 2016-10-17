set(TLSources TransmissionLayer.h TransmissionLayer.cpp)
add_library(TransmissionLayer ${TLSources})

add_subdirectory(AcousticTL)

target_link_libraries(TransmissionLayer AcousticTL)