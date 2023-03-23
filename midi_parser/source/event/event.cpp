#include "event/event.h"
#include <sstream>
#include <iomanip>

namespace MidiParser {
Event::Event(uint64_t delta_time):
	delta_time(delta_time)
{}
//------------------------------------------------------------------------------
Event::Category	Event::get_category(byte status)
{
	if (status == 0xff)
		return META;
	else if ((status >> 4) == 0xf)
		return SYSEX;
	else
		return MIDI;
}
//------------------------------------------------------------------------------
std::string		Event::str() const
{
	std::stringstream ss;
	str(ss);
	return ss.str();
}
//------------------------------------------------------------------------------
std::ostream&	Event::str(std::ostream& os) const
{
	os << std::setw(print_width_delta_time) << delta_time;
	return os;
}
} // MidiParser