#pragma once
#include "common.h"
#include "event/event.h"
#include <vector>
#include <string>

namespace MidiParser {
/*##########################

	MetaEvent

##########################*/
class MetaEvent: public Event
{
public:
	/*---------------------
		enumeration
	---------------------*/
	enum Type
	{
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
		SEQUENCE_SPECIFIC = 0x7f
	};


	/*---------------------
		methods
	---------------------*/
	int				get_status() const override;
	Event::Type		get_event_type() const override;

	virtual
	Type			get_meta_type() const = 0;

	virtual
	std::ostream&		str(std::ostream& os) const override; // {return "";}

	static
	std::shared_ptr<Event>	create(
		uint64_t				delta_time,
		byte					status,
		const byte*&			begin,
		const byte*				end
	);


protected:
	/*---------------------
		members
	---------------------*/
	std::vector<byte>	data;

	/*---------------------
		constructor
	---------------------*/
	MetaEvent(uint64_t delta_time);
	MetaEvent(uint64_t delta_time, const std::vector<byte>& vec);

	/*---------------------
		methods
	---------------------*/
	void			set_string_base(const std::string& str);
	void			set_binary_base(const std::vector<byte>& input);
	std::string		get_string_base() const;
};




/*##########################

	SequenceNumber

##########################*/
class SequenceNumber: public MetaEvent
{
public:
	SequenceNumber(uint64_t delta_time, int sequence_number = 0);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	int				get() const;
	void			set(int sequence_number);
};




/*##########################

	UserText

##########################*/
class UserText: public MetaEvent
{
public:
	UserText(uint64_t delta_time, const std::vector<byte>& vec);
	UserText(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};




/*##########################

	CopyRight

##########################*/
class CopyRight: public MetaEvent
{
public:
	CopyRight(uint64_t delta_time, const std::vector<byte>& vec);
	CopyRight(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};




/*##########################

	TrackName

##########################*/
class TrackName: public MetaEvent
{
public:
	TrackName(uint64_t delta_time, const std::vector<byte>& vec);
	TrackName(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};




/*##########################

	InstrumentName

##########################*/
class InstrumentName: public MetaEvent
{
public:
	InstrumentName(uint64_t delta_time, const std::vector<byte>& vec);
	InstrumentName(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};




/*##########################

	Lyric

##########################*/
class Lyric: public MetaEvent
{
public:
	Lyric(uint64_t delta_time, const std::vector<byte>& vec);
	Lyric(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};




/*##########################

	Marker

##########################*/
class Marker: public MetaEvent
{
public:
	Marker(uint64_t delta_time, const std::vector<byte>& vec);
	Marker(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};




/*##########################

	CuePoint

##########################*/
class CuePoint: public MetaEvent
{
public:
	CuePoint(uint64_t delta_time, const std::vector<byte>& vec);
	CuePoint(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};




/*##########################

	ChannelPrefix

##########################*/
class ChannelPrefix: public MetaEvent
{
public:
	ChannelPrefix(uint64_t delta_time, int channel);

	Type			get_meta_type() const override;
	std::ostream&		str(std::ostream& os) const override;
	int 			get() const;
	void			set(int channel);
};




/*##########################

	MidiPort

##########################*/
class MidiPort: public MetaEvent
{
public:
	MidiPort(uint64_t delta_time, int port);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	int 			get() const;
	void			set(int port);
};




/*##########################

	EndOfTrack

##########################*/
class EndOfTrack: public MetaEvent
{
public:
	EndOfTrack(uint64_t delta_time);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
};




/*##########################

	SetTempo

##########################*/
class SetTempo: public MetaEvent
{
public:
	SetTempo(uint64_t delta_time, int quarter_note_duration);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	microseconds	get_quarter_note_duration() const;
	void			set(int quarter_note_duration); // micro second
};




/*##########################

	SMPTEOffset

##########################*/
class SMPTEOffset: public MetaEvent
{
public:
	SMPTEOffset(
		uint64_t	delta_time, 
		int			hour, 
		int			minute, 
		int			second, 
		int			frame, 
		int			subframe
	);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	int				get_hour() const;
	int				get_minute() const;
	int				get_second() const;
	int				get_frame() const;
	int				get_subframe() const;
	void			set_hour(int hour);
	void			set_minute(int minute);
	void			set_second(int second);
	void			set_frame(int frame);
	void			set_subframe(int subframe);
	void			set(int hour, int minute, int second, int frame, int subframe);
};




/*##########################

	TimeSignature

##########################*/
class TimeSignature: public MetaEvent
{
public:
	TimeSignature(
		uint64_t	delta_time,
		int			numerator,
		int			denominator,
		int			metronome_ticks,
		int			quarter_note_division_32
	);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	int				get_numerator() const;
	int				get_denominator() const;
	int				get_metronome_ticks() const;
	int 			get_quarter_note_division_32() const;
	void			set_numerator(int numerator);
	void			set_denominator(int denominator);
	void			set_metronome_ticks(int metronome_ticks);
	void			set_quarter_note_division_32(int quarter_note_division_32);
	void			set(
		int				numerator,
		int				denominator,
		int				metronome_ticks,
		int				quarter_note_division_32
	);
};




/*##########################

	KeySignature

##########################*/
class KeySignature: public MetaEvent
{
public:
	KeySignature(
		uint64_t delta_time,
		int key,
		int scale
	);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	int				get_key() const;
	int 			get_scale() const;
	void			set_key(int key);
	void			set_scale(int scale);
};




/*##########################

	SequenceSpecific

##########################*/
class SequenceSpecific: public MetaEvent
{
public:
	SequenceSpecific(uint64_t delta_time, const std::vector<byte>& vec);
	SequenceSpecific(uint64_t delta_time, const std::string& str);

	Type			get_meta_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	std::string		get() const;
	void			set(const std::string& str);
};
}