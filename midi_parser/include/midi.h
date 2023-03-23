/*==============================================================================
Warning:
- SMPTE is not supported yet.

Minchang Kim
2023.12
==============================================================================*/

#pragma once
#include "common.h"
#include "chunk.h"
#include "event/midi_event.h"
#include "event/meta_event.h"
#include "event/sysex_event.h"
#include <filesystem>
#include <sstream>
#include <chrono>

namespace MidiParser{
/*##########################

	Division

##########################*/
class Division
{
public:
	enum Type
	{
		QUARTER_NOTE,
		SMPTE,
	};

	Division() = default;
	Division(uint16_t division);
	Division(byte frame_rate, byte ticks);
	
	void			set_division(uint16_t division);
	void			set_smpte(byte frame_rate, byte ticks);
	std::string		to_string() const;
	microseconds	get_delta_time_duration(microseconds quarter_note_duration) const;
	microseconds	get_delta_time_duration() const;

protected:
	Type		type = QUARTER_NOTE;
	uint16_t	value[2] = {0, 0};
};




/*##########################

	Midi

##########################*/
class Midi final
{
public:
	/*---------------------
		enumerations
	---------------------*/
	enum class Format
	{
		SINGLE_TRACK = 0,
		MULTIPLE_TRACK = 1,
		MULTIPLE_SONG = 2
	};

	/*---------------------
		members
	---------------------*/
	Division					division;
	std::vector<Track>			tracks;

	/*---------------------
		constructor
	---------------------*/
	Midi() = default;
	Midi(const std::filesystem::path &file_path);

	/*---------------------
		methods
	---------------------*/
	void					open(const std::filesystem::path &file_path);
	void					close();

	Format					get_format() const;
	int						get_track_count() const;
	std::ostream&			str(std::ostream& os) const;
	std::string				str() const;
	std::filesystem::path	get_file_path() const;
	void					save_str(const std::filesystem::path& file_path) const;
	void					save_str() const;
	void					update_timestamp();
	int						event_count() const;

private:
	/*---------------------
		members
	---------------------*/
	std::filesystem::path	file_path;
};

} // MidiParser