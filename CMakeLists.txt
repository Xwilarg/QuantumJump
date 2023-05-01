cmake_minimum_required(VERSION 3.13)
project(4MBJam)

SET(CMAKE_C_FLAGS "/W4 /EHsc")
aux_source_directory(./src SRCS)
add_executable(4MBJam WIN32 ${SRCS})
target_link_libraries(4MBJam OpenGL32)