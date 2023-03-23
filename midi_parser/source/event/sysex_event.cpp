#include "event/sysex_event.h"
#include "util.h"
#include <stdexcept>

namespace MidiParser {
/*##########################

   SysexEvent

##########################*/
SysexEvent::SysexEvent(uint64_t delta_time):
	Event(delta_time)
{}
//------------------------------------------------------------------------------
Event::Category	SysexEvent::get_category() const
{
	return SYSEX;
}
//------------------------------------------------------------------------------
int 		SysexEvent::get_status() const
{
	return static_cast<int>(get_type());
}
//------------------------------------------------------------------------------
std::ostream&	SysexEvent::str(std::ostream& os) const
{
	return Event::str(os) << " [Sysex]";
}
//------------------------------------------------------------------------------
std::shared_ptr<Event>	SysexEvent::create(
	uint64_t				delta_time,
	byte					status,
	const byte*&			begin,
	const byte*				end	
)
{
	Type type = static_cast<Type>(status);
	switch (type)
	{
		case SYSEX_MESSAGES:
		{
			byte c;
			std::vector<byte> data;
			do
			{
				c = data.emplace_back(read1(begin, end));
			}
			while (c != END_OF_SYSEX_MESSAGES);
			return std::make_shared<SysexMessages>(delta_time, data);
		}
		case MTC_QUARTER_FRAME:
			return std::make_shared<MTCQuarterFrame>(delta_time, read1(begin, end));
		case SONG_POSITION_POINTER:
		{
			byte c[2];
			c[0] = read1(begin, end);
			c[1] = read1(begin, end);
			return std::make_shared<SongPositionPointer>(delta_time, c[0], c[1]);
		}
		case SONG_REQUEST:
			return std::make_shared<SongRequest>(delta_time, read1(begin, end));
		case TUNE_REQUEST:
			return std::make_shared<TuneRequest>(delta_time);
		case END_OF_SYSEX_MESSAGES:
			return std::make_shared<EndOfSysexMessages>(delta_time);
		case TIMING_CLOCK_FOR_SYNC:
			return std::make_shared<TimingClockForSync>(delta_time);
		case START_CURRENT_SEQUENCE:
			return std::make_shared<StartCurrentSequence>(delta_time);
		case CONTINUE_STOPPED_SEQUENCE:
			return std::make_shared<ContinueStoppedSequence>(delta_time);
		case STOP_SEQUENCE:
			return std::make_shared<StopSequence>(delta_time);
		case ACTIVE_SENSING:
			return std::make_shared<ActiveSensing>(delta_time);
	}
	throw std::runtime_error("unknown Sysex Event: " + std::to_string(type));
}




/*##########################

   SysexMessages

##########################*/
SysexMessages::SysexMessages(uint64_t delta_time, const std::vector<byte>& messages):
	SysexEvent(delta_time), messages(messages)
{}
//------------------------------------------------------------------------------
SysexMessages::SysexMessages(uint64_t delta_time, std::vector<byte>&& messages) noexcept:
	SysexEvent(delta_time), messages(std::move(messages))
{}
//------------------------------------------------------------------------------
SysexEvent::Type		SysexMessages::get_type() const
{
	return SYSEX_MESSAGES;
}
//------------------------------------------------------------------------------
std::ostream&			SysexMessages::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Sysex Messages | "
		<< hex_dump(messages);
}
//------------------------------------------------------------------------------
std::vector<byte>		SysexMessages::get_messages() const
{
	return messages;
}
//------------------------------------------------------------------------------
void 					SysexMessages::set_messages(const std::vector<byte>& messages)
{
	this->messages = messages;
}
//------------------------------------------------------------------------------
void 					SysexMessages::set_messages(std::vector<byte>&& messages)
{
	this->messages = std::move(messages);
}




/*##########################

   MTCQuarterFrame

##########################*/
MTCQuarterFrame::MTCQuarterFrame(uint64_t delta_time, byte val):
	SysexEvent(delta_time), value(val)
{}
//------------------------------------------------------------------------------
SysexEvent::Type		MTCQuarterFrame::get_type() const
{
	return MTC_QUARTER_FRAME;
}
//------------------------------------------------------------------------------
std::ostream&			MTCQuarterFrame::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "MTC QuarterFrame | "
		<< get_value();
}
//------------------------------------------------------------------------------
int						MTCQuarterFrame::get_value() const
{
	return value;
}
//------------------------------------------------------------------------------
void 					MTCQuarterFrame::set_value(int val)
{
	this->value = val;
}
//------------------------------------------------------------------------------




/*##########################

   SongPositionPointer

##########################*/
SongPositionPointer::SongPositionPointer(uint64_t delta_time, int position):
	SysexEvent(delta_time), position(position)
{}
//------------------------------------------------------------------------------
SongPositionPointer::SongPositionPointer(uint64_t delta_time, byte b0, byte b1):
	SysexEvent(delta_time)
{
	set_position(b0, b1);
}
//------------------------------------------------------------------------------
SysexEvent::Type	SongPositionPointer::get_type() const
{
	return SONG_POSITION_POINTER;
}
//------------------------------------------------------------------------------
std::ostream&		SongPositionPointer::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Song Pos Ptr | "
		<< get_position();
}
//------------------------------------------------------------------------------
int					SongPositionPointer::get_position() const
{
	return position;
}
//------------------------------------------------------------------------------
void				SongPositionPointer::get_position(byte& b0, byte& b1) const
{
	b0 = (position >> 7) & 0x7f;
	b1 = position & 0x7f;
}
//------------------------------------------------------------------------------
void 				SongPositionPointer::set_position(int position)
{
	this->position = position;
}
//------------------------------------------------------------------------------
void	 			SongPositionPointer::set_position(byte b0, byte b1)
{
	position = ((static_cast<int>(b0) << 7) & 0x7f) | (b1 & 0x7f);
}




/*##########################

   SongRequest

##########################*/
SongRequest::SongRequest(uint64_t delta_time, int song):
	SysexEvent(delta_time)
{
	set_song(song);
}
//------------------------------------------------------------------------------
SysexEvent::Type	SongRequest::get_type() const
{
	return SONG_REQUEST;
}
//------------------------------------------------------------------------------
std::ostream&		SongRequest::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Song Request | ";
}
//------------------------------------------------------------------------------
int					SongRequest::get_song() const
{
	return song;
}
//------------------------------------------------------------------------------
void 				SongRequest::set_song(int song)
{
	this->song = std::clamp(song, 0, 127);
}




/*##########################

   TuneRequest

##########################*/
TuneRequest::TuneRequest(uint64_t delta_time):
	SysexEvent(delta_time)
{}
//------------------------------------------------------------------------------
SysexEvent::Type	TuneRequest::get_type() const
{
	return TUNE_REQUEST;
}
//------------------------------------------------------------------------------
std::ostream&		TuneRequest::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Tune Request | ";
}



/*##########################

   EndOfSysexMessages

##########################*/
EndOfSysexMessages::EndOfSysexMessages(uint64_t delta_time):
	SysexEvent(delta_time)
{}
//------------------------------------------------------------------------------
SysexEvent::Type	EndOfSysexMessages::get_type() const
{
	return END_OF_SYSEX_MESSAGES;
}
//------------------------------------------------------------------------------
std::ostream&		EndOfSysexMessages::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "End Of Sysex Msg | ";
}



/*##########################

   TimingClockForSync

##########################*/
TimingClockForSync::TimingClockForSync(uint64_t delta_time):
	SysexEvent(delta_time)
{}
//------------------------------------------------------------------------------
SysexEvent::Type	TimingClockForSync::get_type() const
{
	return TIMING_CLOCK_FOR_SYNC;
}
//------------------------------------------------------------------------------
std::ostream&		TimingClockForSync::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Timing Clock For Sync | ";
}




/*##########################

   StartCurrentSequence

##########################*/
StartCurrentSequence::StartCurrentSequence(uint64_t delta_time):
	SysexEvent(delta_time)
{}
//------------------------------------------------------------------------------
SysexEvent::Type	StartCurrentSequence::get_type() const
{
	return START_CURRENT_SEQUENCE;
}
//------------------------------------------------------------------------------
std::ostream&		StartCurrentSequence::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Start Current Sequence | ";
}



/*##########################

   ContinueStoppedSequence

##########################*/
ContinueStoppedSequence::ContinueStoppedSequence(uint64_t delta_time):
	SysexEvent(delta_time)
{}
//------------------------------------------------------------------------------
SysexEvent::Type	ContinueStoppedSequence::get_type() const
{
	return CONTINUE_STOPPED_SEQUENCE;
}
//------------------------------------------------------------------------------
std::ostream&		ContinueStoppedSequence::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Continue Stopped Sequence | ";
}



/*##########################

   StopSequence

##########################*/
StopSequence::StopSequence(uint64_t delta_time):
	SysexEvent(delta_time)
{}
//------------------------------------------------------------------------------
SysexEvent::Type	StopSequence::get_type() const
{
	return STOP_SEQUENCE;
}
//------------------------------------------------------------------------------
std::ostream&		StopSequence::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Stop Sequence | ";
}




/*##########################

   ActiveSensing

##########################*/
ActiveSensing::ActiveSensing(uint64_t delta_time):
	SysexEvent(delta_time)
{}
//------------------------------------------------------------------------------
SysexEvent::Type	ActiveSensing::get_type() const
{
	return ACTIVE_SENSING;
}
//------------------------------------------------------------------------------
std::ostream&		ActiveSensing::str(std::ostream& os) const
{
	return
		SysexEvent::str(os)
		<< std::setw(print_width_type)
		<< "Active Sensing | ";
}
} // MidiParser