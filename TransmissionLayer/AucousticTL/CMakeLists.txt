set(ATLSources AucousticTL.h AucousticTL.cpp DtmfSpec.cpp DtmfSpec.h Test/DtmfSpecTest.cpp helperFunctions.h)
add_library(AucousticTL ${ATLSources})

add_subdirectory(PaWrapper)

target_link_libraries(AucousticTL PaWrapper)

add_executable(DtmfSpecTest Test/DtmfSpecTest.cpp)
target_link_libraries(DtmfSpecTest AucousticTL)