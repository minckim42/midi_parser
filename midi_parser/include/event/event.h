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
	enum Category
	{
		META,
		MIDI,
		SYSEX,
	};

	enum Type
	{
		NOTE_OFF = 8,
		NOTE_ON = 9,
		POLYPHONIC_KEY_PRESSURE = 10,
		CONTROL_CHANGE = 11,
		PROGRAM_CHANGE = 12,
		CHANNEL_PRESSURE = 13,
		PITCH_BEND = 14,
		SEQUENCE_NUMBER = 0x00,
		USER_TEXT = 0x01,
		COPY_RIGHT = 0x02,
		TRACK_NAME = 0x03,
		INSTRUMENT_NAME = 0x04,
		LYRIC = 0x05,
		MARKER = 0x06,
		CUE_POINT = 0x07,
		CHANNEL_PREFIX = 0x20,
		MIDI_PORT = 0x21,
		END_OF_TRACK = 0x2f,
		SET_TEMPO = 0x51,
		SMPTE_OFFSET = 0x54,
		TIME_SIGNATURE = 0x58,
		KEY_SIGNATURE = 0x59,
		SEQUENCE_SPECIFIC = 0x7f,
		SYSEX_MESSAGES = 0xf0,
		MTC_QUARTER_FRAME = 0xf1,
		SONG_POSITION_POINTER = 0xf2,
		SONG_REQUEST = 0xf3,
		TUNE_REQUEST = 0xf6,
		END_OF_SYSEX_MESSAGES = 0xf7,
		TIMING_CLOCK_FOR_SYNC = 0xf8,
		START_CURRENT_SEQUENCE = 0xfa,
		CONTINUE_STOPPED_SEQUENCE = 0xfb,
		STOP_SEQUENCE = 0xfc,
		ACTIVE_SENSING = 0xfe,
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
	Category				get_category(byte status);

	virtual 
	Category				get_category() const = 0;

	virtual 
	int						get_status() const = 0;
	
	virtual
	Type					get_type() const = 0;

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