set(TLSources TransmissionLayer.h TransmissionLayer.cpp)
add_library(TransmissionLayer ${TLSources})

add_subdirectory(AucousticTL)

target_link_libraries(TransmissionLayer AucousticTL)