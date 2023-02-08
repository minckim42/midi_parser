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
	virtual std::string		to_string() const = 0;
	
	virtual 
	microseconds	get_delta_time_duration(uint32_t quarter_note_duration) const = 0;
	
	virtual
	microseconds	get_delta_time_duration(microseconds quarter_note_duration) const = 0;

protected:
	Division() = default;
};




/*##########################

	QuarterNoteDivsion

##########################*/
class QuarterNoteDivsion: public Division
{
public:
	uint16_t	division = 96;

	QuarterNoteDivsion(int division = 96);

	std::string			to_string() const override;
	microseconds	get_delta_time_duration(uint32_t quarter_note_duration) const override;
	microseconds	get_delta_time_duration(microseconds quarter_note_duration) const override;
};




/*##########################

	SMPTEDivision

##########################*/
class SMPTEDivision: public Division
{
public:
	byte	frame_rate = 25;
	byte	ticks = 40;
	
	SMPTEDivision(int frame_rate = 25, int ticks = 40);

	std::string			to_string() const override;
	microseconds	get_delta_time_duration(uint32_t quarter_note_duration) const override;
	microseconds	get_delta_time_duration(microseconds quarter_note_duration) const override;
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
	std::shared_ptr<Division>	division;
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