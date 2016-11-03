add_library(StopAndWait StopAndWait.h StopAndWait.cpp)
target_link_libraries(StopAndWait DataLinkLayer)

add_executable(StopAndWaitTest Test/testfile.cpp)
target_link_libraries(StopAndWaitTest StopAndWait)