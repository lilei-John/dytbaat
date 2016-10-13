add_library(DtmfSpec DtmfSpec.h DtmfSpec.cpp)

add_executable(DtmfSpecTest Test/DtmfSpecTest.cpp)
target_link_libraries(DtmfSpecTest DtmfSpec)