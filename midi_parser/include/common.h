#pragma once
#include <cstdint>
#include <chrono>

namespace MidiParser{

typedef uint8_t								byte;
typedef std::chrono::high_resolution_clock	Clock;
typedef std::chrono::microseconds			Microseconds;
typedef Clock::time_point					Timepoint;

}