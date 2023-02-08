#include "event/meta_event.h"
#include "util.h"
#include <sstream>
#include <cstring>
#include <cmath>

namespace MidiParser {
/*##########################

   MetaEvent

##########################*/
MetaEvent::MetaEvent(uint64_t delta_time):
	Event(delta_time)
{}
//------------------------------------------------------------------------------
MetaEvent::MetaEvent(uint64_t delta_time, const std::vector<byte>& vec):
	Event(delta_time), data(vec)
{}
//------------------------------------------------------------------------------
int				MetaEvent::get_status() const
{
	return 0xff;
}
//------------------------------------------------------------------------------
Event::Type		MetaEvent::get_event_type() const
{
	return Event::META;
}
//------------------------------------------------------------------------------
std::ostream&	MetaEvent::str(std::ostream& os) const
{
	return Event::str(os) << " [META] ";
}
//------------------------------------------------------------------------------
void			MetaEvent::set_string_base(const std::string& str)
{
	data.resize(str.size());
	std::memcpy(data.data(), str.data(), str.size());
}
//------------------------------------------------------------------------------
void			MetaEvent::set_binary_base(const std::vector<byte>& input)
{
	data = input;
}
//------------------------------------------------------------------------------
std::string		MetaEvent::get_string_base() const
{
	std::string result;
	for (byte i: data)
	{
		result += static_cast<char>(i);
	}
	return result;
}
//------------------------------------------------------------------------------
std::shared_ptr<Event>	MetaEvent::create(
	uint64_t				delta_time,
	byte					status,
	const byte*&			input,
	const byte*				end
)
{
	Type type = static_cast<Type>(read1(input, end));
	int length = read1(input, end);
	std::vector<byte> tmp(length);
	std::memcpy(tmp.data(), input, length);
	input += length;

	switch (type)
	{
		case SEQUENCE_NUMBER:
			return std::make_shared<SequenceNumber>(delta_time, tmp[0]);
		case USER_TEXT:
			return std::make_shared<UserText>(delta_time, tmp);
		case COPY_RIGHT:
			return std::make_shared<CopyRight>(delta_time, tmp);
		case TRACK_NAME:
			return std::make_shared<TrackName>(delta_time, tmp);
		case INSTRUMENT_NAME:
			return std::make_shared<InstrumentName>(delta_time, tmp);
		case LYRIC:
			return std::make_shared<Lyric>(delta_time, tmp);
		case MARKER:
			return std::make_shared<Marker>(delta_time, tmp);
		case CUE_POINT:
			return std::make_shared<CuePoint>(delta_time, tmp);
		case CHANNEL_PREFIX:
			return std::make_shared<ChannelPrefix>(delta_time, tmp[0]);
		case MIDI_PORT:
			return std::make_shared<MidiPort>(delta_time, tmp[0]);
			break;
		case END_OF_TRACK:
			return std::make_shared<EndOfTrack>(delta_time);
		case SET_TEMPO:
			return std::make_shared<SetTempo>(
						delta_time,
						(static_cast<int>(tmp[0]) << 16) |
						(static_cast<int>(tmp[1]) <<  8) |
						(static_cast<int>(tmp[2]) <<  0)
					);
		case SMPTE_OFFSET:
			return std::make_shared<SMPTEOffset>(
						delta_time, tmp[0], tmp[1], tmp[2], tmp[3], tmp[4]
					);
		case TIME_SIGNATURE:
			return std::make_shared<TimeSignature>(
						delta_time, tmp[0], tmp[1], tmp[2], tmp[3]
					);
		case KEY_SIGNATURE:
			return std::make_shared<KeySignature>(delta_time, tmp[0], tmp[1]);
		case SEQUENCE_SPECIFIC:
			return std::make_shared<SequenceSpecific>(delta_time, tmp);
	}
	throw std::runtime_error("unknown");
}



/*##########################

   SequenceNumber

##########################*/
SequenceNumber::SequenceNumber(uint64_t delta_time, int sequence_number):
	MetaEvent(delta_time)
{
	data.resize(2);
	data.shrink_to_fit();
	set(sequence_number);
}
//------------------------------------------------------------------------------
MetaEvent::Type		SequenceNumber::get_meta_type() const
{
	return SEQUENCE_NUMBER;
}
//------------------------------------------------------------------------------
std::ostream&		SequenceNumber::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "SequenceNumber | " << get();
}
//------------------------------------------------------------------------------
int					SequenceNumber::get() const
{
	const byte* ptr = data.data();
	return read2(ptr, ptr+  2);
}
//------------------------------------------------------------------------------
void				SequenceNumber::set(int sequence_number)
{
	data[0] = (sequence_number >> 8) & 0xff;
	data[1] = (sequence_number) & 0xff;
}




/*##########################

   UserText

##########################*/
UserText::UserText(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
UserText::UserText(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			UserText::get_meta_type() const
{
	return USER_TEXT;
}
//------------------------------------------------------------------------------
std::ostream&			UserText::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "User Text | " << get();
}
//------------------------------------------------------------------------------
std::string		UserText::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			UserText::set(const std::string& str)
{
	set_string_base(str);
}




/*##########################

   CopyRight

##########################*/
CopyRight::CopyRight(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
CopyRight::CopyRight(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			CopyRight::get_meta_type() const
{
	return COPY_RIGHT;
}
//------------------------------------------------------------------------------
std::ostream&			CopyRight::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "Copy Right | " << get();
}
//------------------------------------------------------------------------------
std::string		CopyRight::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			CopyRight::set(const std::string& str)
{
	set_string_base(str);
}




/*##########################

   TrackName

##########################*/
TrackName::TrackName(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
TrackName::TrackName(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			TrackName::get_meta_type() const
{
	return USER_TEXT;
}
//------------------------------------------------------------------------------
std::ostream&			TrackName::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "Track Name | " << get();
}
//------------------------------------------------------------------------------
std::string		TrackName::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			TrackName::set(const std::string& str)
{
	set_string_base(str);
}




/*##########################

   InstrumentName

##########################*/
InstrumentName::InstrumentName(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
InstrumentName::InstrumentName(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			InstrumentName::get_meta_type() const
{
	return INSTRUMENT_NAME;
}
//------------------------------------------------------------------------------
std::ostream&			InstrumentName::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "InstrumentName | " << get();
}
//------------------------------------------------------------------------------
std::string		InstrumentName::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			InstrumentName::set(const std::string& str)
{
	set_string_base(str);
}




/*##########################

   Lyric

##########################*/
Lyric::Lyric(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
Lyric::Lyric(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			Lyric::get_meta_type() const
{
	return INSTRUMENT_NAME;
}
//------------------------------------------------------------------------------
std::ostream&			Lyric::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "Lyric | " << get();
}
//------------------------------------------------------------------------------
std::string		Lyric::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			Lyric::set(const std::string& str)
{
	set_string_base(str);
}




/*##########################

   Marker

##########################*/
Marker::Marker(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
Marker::Marker(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			Marker::get_meta_type() const
{
	return INSTRUMENT_NAME;
}
//------------------------------------------------------------------------------
std::ostream&			Marker::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "Marker | " << get();
}
//------------------------------------------------------------------------------
std::string		Marker::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			Marker::set(const std::string& str)
{
	set_string_base(str);
}




/*##########################

   CuePoint

##########################*/
CuePoint::CuePoint(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
CuePoint::CuePoint(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			CuePoint::get_meta_type() const
{
	return INSTRUMENT_NAME;
}
//------------------------------------------------------------------------------
std::ostream&			CuePoint::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "CuePoint | " << get();
}
//------------------------------------------------------------------------------
std::string		CuePoint::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			CuePoint::set(const std::string& str)
{
	set_string_base(str);
}




/*##########################

   ChannelPrefix

##########################*/
ChannelPrefix::ChannelPrefix(uint64_t delta_time, int channel):
	MetaEvent(delta_time)
{
	data.resize(1);
	data.shrink_to_fit();
	set(channel);
}
//------------------------------------------------------------------------------
MetaEvent::Type			ChannelPrefix::get_meta_type() const
{
	return INSTRUMENT_NAME;
}
//------------------------------------------------------------------------------
std::ostream&			ChannelPrefix::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "ChannelPrefix | " << get();
}
//------------------------------------------------------------------------------
int				ChannelPrefix::get() const
{
	return data[0];
}
//------------------------------------------------------------------------------
void			ChannelPrefix::set(int channel)
{
	data[0] = channel;
}




/*##########################

   MidiPort

##########################*/
MidiPort::MidiPort(uint64_t delta_time, int port):
	MetaEvent(delta_time)
{
	data.resize(1);
	data.shrink_to_fit();
	set(port);
}
//------------------------------------------------------------------------------
MetaEvent::Type			MidiPort::get_meta_type() const
{
	return MIDI_PORT;
}
//------------------------------------------------------------------------------
std::ostream&			MidiPort::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "MidiPort | " << get();
}
//------------------------------------------------------------------------------
int				MidiPort::get() const
{
	return data[0];
}
//------------------------------------------------------------------------------
void			MidiPort::set(int port)
{
	data[0] = port;
}




/*##########################

   EndOfTrack

##########################*/
EndOfTrack::EndOfTrack(uint64_t delta_time):
	MetaEvent(delta_time)
{}
//------------------------------------------------------------------------------
MetaEvent::Type			EndOfTrack::get_meta_type() const
{
	return END_OF_TRACK;
}
//------------------------------------------------------------------------------
std::ostream&			EndOfTrack::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "EndOfTrack";
}




/*##########################

   SetTempo

##########################*/
SetTempo::SetTempo(uint64_t delta_time, int quarter_note_duration):
	MetaEvent(delta_time)
{
	data.resize(3);
	data.shrink_to_fit();
	set(quarter_note_duration);
}
//------------------------------------------------------------------------------
MetaEvent::Type	SetTempo::get_meta_type() const
{
	return SET_TEMPO;
}
//------------------------------------------------------------------------------
std::ostream&			SetTempo::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "SetTempo | " 
		<< get_quarter_note_duration().count() << " ms";
}
//------------------------------------------------------------------------------
microseconds 	SetTempo::get_quarter_note_duration() const
{
	return
		microseconds(
			(static_cast<int>(data[0]) << 16) |
			(static_cast<int>(data[1]) <<  8) |
			static_cast<int>(data[2])
		);
}
//------------------------------------------------------------------------------
void	SetTempo::set(int quarter_note_duration)
{
	data[0] = (quarter_note_duration >> 16) & 0xff;
	data[1] = (quarter_note_duration >> 8) & 0xff;
	data[2] = (quarter_note_duration) & 0xff;
}




/*##########################

   SMPTEOffset

##########################*/
SMPTEOffset::SMPTEOffset(
	uint64_t	delta_time, 
	int			hour, 
	int			minute, 
	int			second, 
	int			frame, 
	int			subframe
):
	MetaEvent(delta_time)
{
	data.resize(5);
	data.shrink_to_fit();
	set(hour, minute, second, frame, subframe);
}

MetaEvent::Type	SMPTEOffset::get_meta_type() const
{
	return SMPTE_OFFSET;
}
//------------------------------------------------------------------------------
std::ostream&			SMPTEOffset::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "SMPTEOffset | " 
		<< get_hour() << ":" << get_minute() << ":" << get_second() 
		<< ", frame:" << get_frame() << ", sub frame: " << get_subframe();
}
//------------------------------------------------------------------------------
int		SMPTEOffset::get_hour() const
{
	return data[0];
}
//------------------------------------------------------------------------------
int		SMPTEOffset::get_minute() const
{
	return data[1];
}
//------------------------------------------------------------------------------
int		SMPTEOffset::get_second() const
{
	return data[2];
}
//------------------------------------------------------------------------------
int		SMPTEOffset::get_frame() const
{
	return data[3];
}
//------------------------------------------------------------------------------
int		SMPTEOffset::get_subframe() const
{
	return data[4];
}
//------------------------------------------------------------------------------
void	SMPTEOffset::set_hour(int hour)
{
	data[0] = hour;
}
//------------------------------------------------------------------------------
void	SMPTEOffset::set_minute(int minute)
{
	data[1] = minute;
}
//------------------------------------------------------------------------------
void	SMPTEOffset::set_second(int second)
{
	data[2] = second;
}
//------------------------------------------------------------------------------
void	SMPTEOffset::set_frame(int frame)
{
	data[3] = frame;
}
//------------------------------------------------------------------------------
void	SMPTEOffset::set_subframe(int subframe)
{
	data[4] = subframe;
}
//------------------------------------------------------------------------------
void	SMPTEOffset::set(int hour, int minute, int second, int frame, int subframe)
{
	set_hour(hour);
	set_minute(minute);
	set_second(second);
	set_frame(frame);
	set_subframe(subframe);
}
//------------------------------------------------------------------------------



/*##########################

   TimeSignature

##########################*/
TimeSignature::TimeSignature(
	uint64_t delta_time,
	int numerator,
	int denominator,
	int metronome_ticks,
	int quarter_note_division_32
):
	MetaEvent(delta_time)
{
	data.resize(5);
	data.shrink_to_fit();
	set(numerator, denominator, metronome_ticks, quarter_note_division_32);
}
//------------------------------------------------------------------------------
MetaEvent::Type	TimeSignature::get_meta_type() const
{
	return TIME_SIGNATURE;
}
//------------------------------------------------------------------------------
std::ostream&			TimeSignature::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "TimeSignature | "
		<< get_numerator() << "/" << pow(2, get_denominator()) 
		<< ", metronome ticks: " << get_metronome_ticks()
		<< " (1/4) / (1/32) = " << get_quarter_note_division_32();
}
//------------------------------------------------------------------------------
int		TimeSignature::get_numerator() const
{
	return data[0];
}
//------------------------------------------------------------------------------
int		TimeSignature::get_denominator() const
{
	return data[1];
}
//------------------------------------------------------------------------------
int		TimeSignature::get_metronome_ticks() const
{
	return data[2];
}
//------------------------------------------------------------------------------
int		TimeSignature::get_quarter_note_division_32() const
{
	return data[3];
}
//------------------------------------------------------------------------------
void	TimeSignature::set_numerator(int numerator)
{
	data[0] = numerator;
}
//------------------------------------------------------------------------------
void	TimeSignature::set_denominator(int denominator)
{
	data[1] = denominator;
}
//------------------------------------------------------------------------------
void	TimeSignature::set_metronome_ticks(int metronome_ticks)
{
	data[2] = metronome_ticks;
}
//------------------------------------------------------------------------------
void	TimeSignature::set_quarter_note_division_32(int quarter_note_division_32)
{
	data[3] = quarter_note_division_32;
}
//------------------------------------------------------------------------------
void	TimeSignature::set(
	int		numerator, 
	int		denominator, 
	int		metronome_ticks, 
	int		quarter_note_division_32
)
{
	set_numerator(numerator);
	set_denominator(denominator);
	set_metronome_ticks(metronome_ticks);
	set_quarter_note_division_32(quarter_note_division_32);
};




/*##########################

   KeySignature

##########################*/
KeySignature::KeySignature(
	uint64_t delta_time,
	int key,
	int scale
):
	MetaEvent(delta_time)
{
	data.resize(2);
	data.shrink_to_fit();
}
//------------------------------------------------------------------------------
MetaEvent::Type	KeySignature::get_meta_type() const
{
	return KEY_SIGNATURE;
}
//------------------------------------------------------------------------------
std::ostream&			KeySignature::str(std::ostream& os) const
{
	MetaEvent::str(os)
	<< std::setw(print_width_type) << "KeySignature | ";
	if (get_key() > 0)
		os << "Sharp: " << get_key();
	else if (get_key() < 0)
		os << "flat: " << get_key();
	else
		os << "natural";
	os << " " << (get_scale() == 1 ? "Major" : "Minor");
	return os;
}
//------------------------------------------------------------------------------
int		KeySignature::get_key() const
{
	return static_cast<int8_t>(data[0]);
}
//------------------------------------------------------------------------------
int	KeySignature::get_scale() const
{
	return data[1];
}
//------------------------------------------------------------------------------
void	KeySignature::set_key(int key)
{
	data[0] = key;
}
//------------------------------------------------------------------------------
void	KeySignature::set_scale(int scale)
{
	data[1] = scale;
}
//------------------------------------------------------------------------------



/*##########################

   SequenceSpecific

##########################*/
SequenceSpecific::SequenceSpecific(uint64_t delta_time, const std::vector<byte>& vec):
	MetaEvent(delta_time, vec)
{}
//------------------------------------------------------------------------------
SequenceSpecific::SequenceSpecific(uint64_t delta_time, const std::string& str):
	MetaEvent(delta_time)
{
	set(str);
}
//------------------------------------------------------------------------------
MetaEvent::Type			SequenceSpecific::get_meta_type() const
{
	return SEQUENCE_SPECIFIC;
}
//------------------------------------------------------------------------------
std::ostream&			SequenceSpecific::str(std::ostream& os) const
{
	return
		MetaEvent::str(os)
		<< std::setw(print_width_type) << "Sequence Specific | " << hex_dump(data);
}
//------------------------------------------------------------------------------
std::string		SequenceSpecific::get() const
{
	return get_string_base();
}
//------------------------------------------------------------------------------
void			SequenceSpecific::set(const std::string& str)
{
	set_string_base(str);
}
//------------------------------------------------------------------------------


} // MidiParser










































