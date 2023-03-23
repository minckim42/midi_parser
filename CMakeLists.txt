cmake_minimum_required(VERSION 3.0)
project(midi_example)

set(CMAKE_CXX_STANDARD 20)

add_executable(midi_example
	example.cpp
	midi_out/midi_out.cpp
)

add_subdirectory(midi_parser)

target_include_directories(midi_example PRIVATE
	include
	midi_parser/include
)

target_link_libraries(midi_example PRIVATE
	winmm
	midi_parser
)


target_compile_definitions(midi_example PRIVATE
	ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
)