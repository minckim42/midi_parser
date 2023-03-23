#pragma once
#include "common.h"
#include "event/event.h"
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace MidiParser {
class Track final
{
public:
	/*---------------------
		members
	---------------------*/
	std::vector<std::shared_ptr<Event>> events;

	/*---------------------
		constructors
	---------------------*/
	Track(const uint8_t*& input, const uint8_t* end);
	Track() = default;
};
}