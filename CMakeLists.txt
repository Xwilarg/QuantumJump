cmake_minimum_required(VERSION 3.13)
project(4MBJam)

SET(CMAKE_C_FLAGS "/W4")

include_directories(./inc/ SYSTEM $ENV{DXSDK_DIR}Include)
link_directories($ENV{DXSDK_DIR}Lib/x86)
aux_source_directory(./src SRCS)
aux_source_directory(./src/component SRCS)
aux_source_directory(./src/rendering SRCS)
aux_source_directory(./src/context SRCS)
aux_source_directory(./src/utils SRCS)
aux_source_directory(./src/audio SRCS)

add_executable(4MBJam WIN32 ${SRCS})

target_link_libraries(4MBJam d3d9 d3dx9 xaudio2)