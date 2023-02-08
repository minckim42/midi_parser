cmake_minimum_required(VERSION 3.0)
project(midi_parser)

set(CMAKE_CXX_STANDARD 20)

add_library(midi_parser
	source/event/controller.cpp
	source/event/event.cpp
	source/event/instrument.cpp
	source/event/meta_event.cpp
	source/event/midi_event.cpp
	source/event/note.cpp
	source/event/sysex_event.cpp
	source/midi.cpp
	source/util.cpp
)

target_include_directories(midi_parser PRIVATE
	include
)