#pragma once
#include "common.h"
#include <cstdint>
#include <memory>
#include <string>

namespace MidiParser {
class Event
{
public:
	/*---------------------
		typedef
	---------------------*/
	typedef std::shared_ptr<Event>			ptr;
	typedef std::shared_ptr<const Event>	const_ptr;

	/*---------------------
		enumerations
	---------------------*/
	enum Type
	{
		META,
		MIDI,
		SYSEX,
	};

	/*---------------------
		members
	---------------------*/
	uint64_t				delta_time = 0;
	uint64_t				timestamp = 0;

	/*---------------------
		methods
	---------------------*/
	static 
	std::shared_ptr<Event>	create();

	static
	Type					get_event_type(byte status);

	virtual 
	Type					get_event_type() const = 0;

	virtual 
	int						get_status() const = 0;
	
	virtual 
	std::string				str() const;

	virtual 
	std::ostream&			str(std::ostream& os) const;

	// virtual
	// std::ostream&			binary(std::ostream& os);


protected:
	/*---------------------
		constructors
	---------------------*/
	Event(uint64_t delta_time);
	Event() = default;
	Event(const Event&) = default;
	Event(Event&&) noexcept = default;
	Event& operator=(const Event&) = default;
	Event& operator=(Event&&) noexcept = default;
	~Event() = default;

	/*---------------------
		members
	---------------------*/
	const int		print_width_type = 20;
	const int		print_width_delta_time = 5;
};

// std::ostream&		operator<<(std::ostream& os, const Event& event);

} // MidiParser