#include "midi.h"
#include "util.h"
#include <vector>
#include <iostream>
#include <bit>



namespace MidiParser {
/*##########################

	Division

##########################*/
Division::Division(uint16_t division)
{
	set_division(division);
}
//------------------------------------------------------------------------------
Division::Division(byte frame_rate, byte ticks)
{
	set_smpte(frame_rate, ticks);
}
//------------------------------------------------------------------------------
void			Division::set_division(uint16_t division)
{
	type = QUARTER_NOTE;
	value[0] = division;
	value[1] = 0;
}
//------------------------------------------------------------------------------
void			Division::set_smpte(byte frame_rate, byte ticks)
{
	type = SMPTE;
	value[0] = frame_rate;
	value[1] = ticks;
}
//------------------------------------------------------------------------------
std::string		Division::to_string() const
{
	std::stringstream ss;
	if (type == QUARTER_NOTE)
		ss << "Quarter note division: " << value[0];
	else
		ss << "Frames per second: " << value[0] << ", Ticks per frame: " << value[1];
	return ss.str();
}
//------------------------------------------------------------------------------
Microseconds	Division::get_delta_time_duration(Microseconds quarter_note_duration) const
{
	if (type == QUARTER_NOTE)
		return quarter_note_duration / value[0];
	return Microseconds(0);
}
//------------------------------------------------------------------------------
Microseconds	Division::get_delta_time_duration() const
{
	if (type == SMPTE)
		return Microseconds(1000000 / value[0] / value[1]);
	return Microseconds(0);
}





/*##########################

	Midi

##########################*/
Midi::Midi(const std::filesystem::path& file_path)
{
	open(file_path);
}
//------------------------------------------------------------------------------
void	Midi::open(const std::filesystem::path& file_path)
{
	close();
	std::vector<byte> data = read_bin_file(file_path);

	const byte* begin = &data[0];
	const byte* end = begin + data.size();

	std::string magic_num = "0000";
	magic_num[0] = read1(begin, end);
	magic_num[1] = read1(begin, end);
	magic_num[2] = read1(begin, end);
	magic_num[3] = read1(begin, end);
	if (magic_num != "MThd")
		throw std::runtime_error("Invalid file");

	// length of header data
	read4(begin, end);
	
	// format
	read2(begin, end);

	// track count
	uint16_t track_count = read2(begin, end);
	tracks.reserve(track_count);
	
	// division
	int16_t div = read2(begin, end);
	if (div < 0)
	{
		if constexpr(std::endian::native == std::endian::little)
			division.set_smpte(-(div >> 8), div & 0xff);
		else
			division.set_smpte(-(div & 0xff), div >> 8);
	}
	else
	{
		division.set_division(div);
	}

	for (int i = 0; i < track_count; i++)
	{
		magic_num[0] = read1(begin, end);
		magic_num[1] = read1(begin, end);
		magic_num[2] = read1(begin, end);
		magic_num[3] = read1(begin, end);
		if (magic_num != "MTrk")
			throw std::runtime_error("Invalid file");
		uint32_t length = read4(begin, end);
		const byte* track_end = begin + length;
		if (track_end > end)
			throw std::out_of_range("");

		Track& track = tracks.emplace_back();
		track.events.reserve(length / 10);

		std::shared_ptr<Event> prev_event;
		while (begin < track_end)
		{
			uint64_t delta_time = read_variable(begin, track_end);
			uint64_t status = read1(begin, track_end);
			

			if ((status >> 4) < 8)
			{
				--begin;
				if (!prev_event)
					throw std::runtime_error("Invalid file");
				status = prev_event->get_status();
			}
			Event::Category type = Event::get_category(status);
			
			
			if (type == Event::META)
			{
				prev_event = track.events.emplace_back(
					MetaEvent::create(delta_time, status, begin, track_end)
				);
				auto meta_type = dynamic_cast<MetaEvent*>(prev_event.get())->get_type();
				if (meta_type == MetaEvent::END_OF_TRACK)
				{
					begin = track_end;
					break;
				}
			}
			else if (type == Event::SYSEX)
			{
				prev_event = track.events.emplace_back(
					SysexEvent::create(delta_time, status, begin, track_end)
				);
			}
			else 
			{

				prev_event = track.events.emplace_back(
					MidiEvent::create(delta_time, status, begin, track_end)
				);
			}
		}
	}
	update_timestamp();
	this->file_path = file_path;
}
//------------------------------------------------------------------------------
void			Midi::close()
{
	tracks.clear();
	file_path.clear();
}
//------------------------------------------------------------------------------
std::ostream&	Midi::str(std::ostream& os) const
{
	os 
	<< "Header\n"
	<< "Format: ";
	if (tracks.size() == 1)
		os << "Single Track";
	else if (tracks.size() > 1)
		os << "Multiple Tracks";
	os
	<< "Num of tracks: " << tracks.size() << '\n'
	<< "Division: " << division.to_string() << '\n'
	<< "\n===================================================================\n";
	
	int track_num = 0;
	for (const auto& track: tracks)
	{
		os 
		<< "Track " << track_num++
		<< "\nNum of Events: " << track.events.size() << '\n';
		
		int event_num = 0;
		for (const auto& event: track.events)
		{
			os << std::setw(6) << event_num++ << " | ";
			event->str(os) << '\n';
		}
		os<< "===================================================================\n";
	}
	return os;
}
//------------------------------------------------------------------------------
std::string		Midi::str() const
{
	std::stringstream ss;
	str(ss);
	return ss.str();
}
//------------------------------------------------------------------------------
void			Midi::save_str(const std::filesystem::path& file_path) const
{
	std::ofstream ofs(file_path);
	if (!ofs.is_open())
		throw std::runtime_error("create failed");

	str(ofs);
	ofs.close();
}
//------------------------------------------------------------------------------
void			Midi::save_str() const
{
	std::filesystem::path save_path = file_path;
	save_path.replace_extension("txt");

	std::ofstream ofs(save_path);
	if (!ofs.is_open())
		throw std::runtime_error("create failed");

	str(ofs);
	ofs.close();
}
//------------------------------------------------------------------------------
void			Midi::update_timestamp()
{
	for (Track& track :tracks)
	{
		uint64_t timestamp = 0;
		for (Event::ptr& event: track.events)
		{
			timestamp += event->delta_time;
			event->timestamp = timestamp;
		}
	}
}
//------------------------------------------------------------------------------
int				Midi::event_count() const
{
	int count = 0;
	for (const Track& track: tracks)
	{
		count += track.events.size();
	}
	return count;
}
} // MidiParser