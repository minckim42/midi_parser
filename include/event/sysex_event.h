#pragma once
#include "common.h"
#include "event.h"
#include <string>
#include <vector>
#include <memory>

namespace MidiParser {
/*##########################

	SysexEvent

##########################*/
class SysexEvent : public Event
{
public:
	/*---------------------
		enumeration
	---------------------*/
	enum Type
	{
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
		methods
	---------------------*/
	virtual 
	int				get_status() const override;
	
	virtual 
	Type			get_sysex_type() const = 0;
	
	virtual
	std::ostream&	str(std::ostream& os) const override;

	Event::Type 	get_event_type() const override;

	static 
	std::shared_ptr<Event>	create(
		uint64_t				delta_time,
		byte					status,
		const byte*&			begin,
		const byte*				end
	);


protected:
	/*---------------------
		constructor
	---------------------*/
	SysexEvent(uint64_t delta_time);
	SysexEvent() = default;
	SysexEvent(const SysexEvent &) = default;
	SysexEvent(SysexEvent &&) noexcept = default;
	~SysexEvent() = default;
	SysexEvent&		operator=(const SysexEvent &) = default;
	SysexEvent&		operator=(SysexEvent &&) noexcept = default;
};




/*##########################

	SysexMessages

##########################*/
class SysexMessages final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	SysexMessages() = default;
	SysexMessages(uint64_t delta_time, const std::vector<byte>& messages);
	SysexMessages(uint64_t delta_time, std::vector<byte>&& messages) noexcept;

	/*---------------------
		methods
	---------------------*/
	Type				get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
	std::vector<byte>	get_messages() const;
	void				set_messages(const std::vector<byte>& messages);
	void				set_messages(std::vector<byte>&& messages);
	


private:
	/*---------------------
		members
	---------------------*/
	std::vector<byte>	messages;
};




/*##########################

	MTCQuarterFrame

##########################*/
class MTCQuarterFrame final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	MTCQuarterFrame() = default;
	MTCQuarterFrame(uint64_t delta_time, byte val);

	/*---------------------
		methods
	---------------------*/
	Type				get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
	int 				get_value() const;
	void				set_value(int val);


private:
	/*---------------------
		members
	---------------------*/
	byte				value;
};




/*##########################

	SongPositionPointer

##########################*/
class SongPositionPointer final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	SongPositionPointer() = default;
	SongPositionPointer(uint64_t delta_time, int position);
	SongPositionPointer(uint64_t delta_time, byte b0, byte b1);

	/*---------------------
		methods
	---------------------*/
	Type		get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
	int 		get_position() const;
	void 		get_position(byte& b0, byte& b1) const;
	void		set_position(int val);
	void	 	set_position(byte b0, byte b1);

private:
	/*---------------------
		members
	---------------------*/
	uint16_t	position;
};




/*##########################

	SongRequest

##########################*/
class SongRequest final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	SongRequest() = default;
	SongRequest(uint64_t delta_time, int song);

	/*---------------------
		methods
	---------------------*/
	Type				get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
	int 				get_song() const;
	void	 			set_song(int song);

private:
	/*---------------------
		members
	---------------------*/
	byte		song;
};




/*##########################

	TuneRequest

##########################*/
class TuneRequest final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	TuneRequest() = default;
	TuneRequest(uint64_t delta_time);

	/*---------------------
		methods
	---------------------*/
	Type				get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
};




/*##########################

	EndOfSysexMessages

##########################*/
class EndOfSysexMessages final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	EndOfSysexMessages() = default;
	EndOfSysexMessages(uint64_t delta_time);

	/*---------------------
		methods
	---------------------*/
	Type		get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
};




/*##########################

	TimingClockForSync

##########################*/
class TimingClockForSync final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	TimingClockForSync() = default;
	TimingClockForSync(uint64_t delta_time);

	/*---------------------
		methods
	---------------------*/
	Type		get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
};




/*##########################

	StartCurrentSequence

##########################*/
class StartCurrentSequence final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	StartCurrentSequence() = default;
	StartCurrentSequence(uint64_t delta_time);

	/*---------------------
		methods
	---------------------*/
	Type		get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
};




/*##########################

	ContinueStoppedSequence

##########################*/
class ContinueStoppedSequence final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	ContinueStoppedSequence() = default;
	ContinueStoppedSequence(uint64_t delta_time);

	/*---------------------
		methods
	---------------------*/
	Type		get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
};




/*##########################

	StopSequence

##########################*/
class StopSequence final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	StopSequence() = default;
	StopSequence(uint64_t delta_time);

	/*---------------------
		methods
	---------------------*/
	Type		get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
};




/*##########################

	ActiveSensing

##########################*/
class ActiveSensing final : public SysexEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	ActiveSensing() = default;
	ActiveSensing(uint64_t delta_time);

	/*---------------------
		methods
	---------------------*/
	Type		get_sysex_type() const override;
	std::ostream&		str(std::ostream& os) const override;
};
} // MidiParser





