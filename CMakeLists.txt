cmake_minimum_required(VERSION 3.13)
project(QuantumRanger)

SET(CMAKE_C_FLAGS "/W4")

include_directories(./inc/)
aux_source_directory(./src SRCS)
aux_source_directory(./src/component SRCS)
aux_source_directory(./src/rendering SRCS)
aux_source_directory(./src/context SRCS)
aux_source_directory(./src/utils SRCS)
aux_source_directory(./src/audio SRCS)

add_executable(QuantumRanger WIN32 ${SRCS})

target_link_libraries(QuantumRanger d3d9 xaudio2)

file(COPY res DESTINATION .)