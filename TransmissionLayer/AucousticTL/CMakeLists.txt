set(ATLSources AucousticTL.h AucousticTL.cpp)
add_library(AucousticTL ${ATLSources})

add_subdirectory(PaWrapper)

target_link_libraries(AucousticTL PaWrapper)