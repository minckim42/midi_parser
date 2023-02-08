#include "event/midi_event.h"
#include "util.h"
#include <sstream>

namespace MidiParser {
/*##########################

	MidiEvent

##########################*/
MidiEvent::MidiEvent(uint64_t delta_time, int channel):
	Event(delta_time), channel(channel)
{}
//------------------------------------------------------------------------------
Event::Type		MidiEvent::get_event_type() const
{
	return Event::MIDI;
}
//------------------------------------------------------------------------------
int				MidiEvent::get_channel() const
{
	return channel;
}
//------------------------------------------------------------------------------
std::ostream&	MidiEvent::str(std::ostream& os) const
{
	return Event::str(os) << " [MIDI] ";
}
//------------------------------------------------------------------------------
void			MidiEvent::set_channel(int channel)
{
	this->channel = std::clamp(channel, 0, 15);
}
//------------------------------------------------------------------------------
Note			MidiEvent::get_note_base() const
{
	return Note(data[0]);
}
//------------------------------------------------------------------------------
void			MidiEvent::set_note_base(Note note)
{
	data[0] = note.get_note_number();
}
//------------------------------------------------------------------------------
void			MidiEvent::set_data0(int data0)
{
	data[0] = std::clamp(data0, 0, 127);
}
//------------------------------------------------------------------------------
void			MidiEvent::set_data1(int data1)
{
	data[1] = std::clamp(data1, 0, 127);
}
//------------------------------------------------------------------------------
std::shared_ptr<Event>	MidiEvent::create(
	uint64_t				delta_time,
	int						status,
	const byte*&			begin,
	const byte*				end
)
{
	Type type = static_cast<Type>((status >> 4) & 0xf);
	int channel = status & 0xf;
	byte val0 = read1(begin, end);
	byte val1 = 0;
	if (type != PROGRAM_CHANGE && type != CHANNEL_PRESSURE)
		val1 = read1(begin, end);
	switch (type)
	{
		case NOTE_OFF:
			return std::make_shared<NoteOff>(delta_time, channel, val0, val1);
		case NOTE_ON:
			return std::make_shared<NoteOn>(delta_time, channel, val0, val1);
		case POLYPHONIC_KEY_PRESSURE:
			return std::make_shared<PolyphonicKeyPressure>(delta_time, channel, val0, val1);
		case CONTROL_CHANGE:
			return std::make_shared<ControlChange>(delta_time, channel, val0, val1);
		case PROGRAM_CHANGE:
			return std::make_shared<ProgramChange>(delta_time, channel, val0);
		case CHANNEL_PRESSURE:
			return std::make_shared<ChannelPressure>(delta_time, channel, val0);
		case PITCH_BEND:
			return std::make_shared<PitchBend>(delta_time, channel, val0, val1);
	}
	throw std::runtime_error("unknown" + std::to_string(type));
}



/*##########################

	NoteOff

##########################*/
NoteOff::NoteOff(
	uint64_t	delta_time,
	int			channel,
	Note		note,
	int			velocity
):
	MidiEvent(delta_time, channel)
{
	set_note(note);
	set_velocity(velocity);
}
//------------------------------------------------------------------------------
int		NoteOff::get_status() const
{
	return 0x80 | get_channel();
}
//------------------------------------------------------------------------------
MidiEvent::Type		NoteOff::get_midi_type() const
{
	return NOTE_OFF;
}
//------------------------------------------------------------------------------
std::ostream&		NoteOff::str(std::ostream& os) const
{
	return 
		MidiEvent::str(os)
		<< std::setw(print_width_type) 
		<< "Note Off | " 
		<< "ch: " << std::setw(2) << get_channel() << ": "
		<< get_note().to_string() << " " << get_velocity();
}
//------------------------------------------------------------------------------
Note	NoteOff::get_note() const
{
	return get_note_base();
}
//------------------------------------------------------------------------------
int		NoteOff::get_velocity() const
{
	return data[1];
}
//------------------------------------------------------------------------------
void	NoteOff::set_note(Note note)
{
	set_note_base(note);
}
//------------------------------------------------------------------------------
void	NoteOff::set_note(int note_number)
{
	set_note_base(note_number);
}
//------------------------------------------------------------------------------
void	NoteOff::set_note(Pitch pitch, Octave octave)
{
	set_note_base(Note(pitch, octave));
}
//------------------------------------------------------------------------------
void	NoteOff::set_velocity(int velocity)
{
	set_data1(velocity);
}
//------------------------------------------------------------------------------
uint32_t	NoteOff::get_binary() const
{
	return get_binary_base_2();
}



/*##########################

	NoteOn

##########################*/
NoteOn::NoteOn(
	uint64_t	delta_time,
	int			channel,
	Note		note,
	int			velocity
):
	MidiEvent(delta_time, channel)
{
	set_note(note);
	set_velocity(velocity);
}
//------------------------------------------------------------------------------
int		NoteOn::get_status() const
{
	return 0x90 | get_channel();
}
//------------------------------------------------------------------------------
MidiEvent::Type		NoteOn::get_midi_type() const
{
	return NOTE_ON;
}
//------------------------------------------------------------------------------
std::ostream&		NoteOn::str(std::ostream& os) const
{
	return 
		MidiEvent::str(os)
		<< std::setw(print_width_type) 
		<< "Note On | " 
		<< "ch: " << std::setw(2) << get_channel() << ": "
		<< get_note().to_string() << " " << get_velocity();
}
//------------------------------------------------------------------------------
Note	NoteOn::get_note() const
{
	return get_note_base();
}
//------------------------------------------------------------------------------
int		NoteOn::get_velocity() const
{
	return data[1];
}
//------------------------------------------------------------------------------
void	NoteOn::set_note(Note note)
{
	set_note_base(note);
}
//------------------------------------------------------------------------------
void	NoteOn::set_note(int note_number)
{
	set_note_base(note_number);
}
//------------------------------------------------------------------------------
void	NoteOn::set_note(Pitch pitch, Octave octave)
{
	set_note_base(Note(pitch, octave));
}
//------------------------------------------------------------------------------
void	NoteOn::set_velocity(int velocity)
{
	set_data1(velocity);
}
//------------------------------------------------------------------------------
uint32_t	NoteOn::get_binary() const
{
	return get_binary_base_2();
}




/*##########################

	PolyphonicKeyPressure

##########################*/
PolyphonicKeyPressure::PolyphonicKeyPressure(
	uint64_t	delta_time,
	int			channel,
	Note		note,
	int			pressure
):
	MidiEvent(delta_time, channel)
{
	set_note(note);
	set_pressure(pressure);
}
//------------------------------------------------------------------------------
int		PolyphonicKeyPressure::get_status() const
{
	return 0x90 | get_channel();
}
//------------------------------------------------------------------------------
MidiEvent::Type		PolyphonicKeyPressure::get_midi_type() const
{
	return POLYPHONIC_KEY_PRESSURE;
}
//------------------------------------------------------------------------------
std::ostream&		PolyphonicKeyPressure::str(std::ostream& os) const
{
	return
		MidiEvent::str(os)
		<< std::setw(print_width_type) 
		<< " Polyphonic Key Pressure | " 
		<< "ch: " << std::setw(2) << get_channel() << ": "
		<< get_note().to_string() << " " << get_pressure();
}
//------------------------------------------------------------------------------
Note	PolyphonicKeyPressure::get_note() const
{
	return get_note_base();
}
//------------------------------------------------------------------------------
int		PolyphonicKeyPressure::get_pressure() const
{
	return data[1];
}
//------------------------------------------------------------------------------
void	PolyphonicKeyPressure::set_note(Note note)
{
	set_note_base(note);
}
//------------------------------------------------------------------------------
void	PolyphonicKeyPressure::set_note(int note_number)
{
	set_note_base(Note(note_number));
}
//------------------------------------------------------------------------------
void	PolyphonicKeyPressure::set_note(Pitch pitch, Octave octave)
{
	set_note_base(Note(pitch, octave));
}
//------------------------------------------------------------------------------
void	PolyphonicKeyPressure::set_pressure(int pressure)
{
	set_data1(pressure);
}
//------------------------------------------------------------------------------
uint32_t	PolyphonicKeyPressure::get_binary() const
{
	return get_binary_base_2();
}




/*##########################

	ControlChange

##########################*/
ControlChange::ControlChange(
	uint64_t			delta_time,
	int					channel,
	Controller			controller,
	int					value
):
	MidiEvent(delta_time, channel)
{
	set_controller(controller);
	set_value(value);
}
//------------------------------------------------------------------------------
int					ControlChange::get_status() const
{
	return 0xb0 | get_channel();
}
//------------------------------------------------------------------------------
MidiEvent::Type		ControlChange::get_midi_type() const
{
	return CONTROL_CHANGE;
}
//------------------------------------------------------------------------------
std::ostream&		ControlChange::str(std::ostream& os) const
{
	return
		MidiEvent::str(os)
		<< std::setw(print_width_type) 
		<< "Control Change | " 
		<< "ch: " << std::setw(2) << get_channel() << ": "
		<< std::setw(20) << get_controller().get_type_name() << " "<< get_value();
}
//------------------------------------------------------------------------------
Controller			ControlChange::get_controller() const
{
	return Controller(static_cast<Controller::Type>(data[0]));
}
//------------------------------------------------------------------------------
int					ControlChange::get_value() const
{
	return data[1];
}
//------------------------------------------------------------------------------
void				ControlChange::set_controller(Controller controller)
{
	set_data0(controller.get_type());
}
//------------------------------------------------------------------------------
void				ControlChange::set_value(int value)
{
	set_data1(value);
}
//------------------------------------------------------------------------------
uint32_t	ControlChange::get_binary() const
{
	return get_binary_base_2();
}




/*##########################

	ProgramChange

##########################*/
ProgramChange::ProgramChange(
	uint64_t	delta_time,
	int			channel,
	Instrument	instrument
):
	MidiEvent(delta_time, channel)
{
	set_instrument(instrument);
}
//------------------------------------------------------------------------------
int		ProgramChange::get_status() const
{
	return 0xc0 | get_channel();
}
//------------------------------------------------------------------------------
MidiEvent::Type		ProgramChange::get_midi_type() const
{
	return PROGRAM_CHANGE;
}
//------------------------------------------------------------------------------
std::ostream&		ProgramChange::str(std::ostream& os) const
{
	return 
		MidiEvent::str(os)
		<< std::setw(print_width_type) 
		<< "Program Change | " 
		<< "ch: " << std::setw(2) << get_channel() << ": "
		<< std::setw(20) << get_instrument().get_type_name();
}
//------------------------------------------------------------------------------
Instrument		ProgramChange::get_instrument() const
{
	return Instrument(static_cast<Instrument::Type>(data[0]));
}
//------------------------------------------------------------------------------
void	ProgramChange::set_instrument(Instrument instrument)
{
	set_data0(instrument.get_type());
}
//------------------------------------------------------------------------------
uint32_t	ProgramChange::get_binary() const
{
	return get_binary_base_1();
}




/*##########################

	ChannelPressure

##########################*/
ChannelPressure::ChannelPressure(
	uint64_t delta_time, 
	int channel, 
	int pressure
): 
	MidiEvent(delta_time, channel)
{
	set_pressure(pressure);
}
//------------------------------------------------------------------------------
int ChannelPressure::get_status() const
{
	return 0xd0 | get_channel();
}
//------------------------------------------------------------------------------
MidiEvent::Type		ChannelPressure::get_midi_type() const
{
	return CHANNEL_PRESSURE;
}
//------------------------------------------------------------------------------
std::ostream&		ChannelPressure::str(std::ostream& os) const
{
	return
		MidiEvent::str(os)
		<< std::setw(print_width_type) 
		<< "Channel Pressure | " 
		<< "ch: " << std::setw(2) << get_channel() << ": "
		<< get_pressure();
}
//------------------------------------------------------------------------------
int		ChannelPressure::get_pressure() const
{
	return data[0];
}
//------------------------------------------------------------------------------
void	ChannelPressure::set_pressure(int pressure)
{
	set_data1(pressure);
}
//------------------------------------------------------------------------------
uint32_t	ChannelPressure::get_binary() const
{
	return get_binary_base_1();
}




/*##########################

	PitchBend

##########################*/
PitchBend::PitchBend(
	uint64_t	delta_time,
	int			channel,
	int			lsb,
	int			msb
):
	MidiEvent(delta_time, channel)
{
	set_lsb(lsb);
	set_msb(msb);
}
//------------------------------------------------------------------------------
int PitchBend::get_status() const
{
	return 0xe0 | get_channel();
}
//------------------------------------------------------------------------------
MidiEvent::Type PitchBend::get_midi_type() const
{
	return PITCH_BEND;
}
//------------------------------------------------------------------------------
std::ostream&		PitchBend::str(std::ostream& os) const
{
	return
		MidiEvent::str(os)
		<< std::setw(print_width_type) 
		<< "Pitch Bend | " 
		<< "ch: " << std::setw(2) << get_channel() << ": "
		<< get_lsb() << ", " << get_msb();
}
//------------------------------------------------------------------------------
int		PitchBend::get_lsb() const
{
	return data[0];
}
//------------------------------------------------------------------------------
int		PitchBend::get_msb() const
{
	return data[1];
}
//------------------------------------------------------------------------------
void	PitchBend::set_lsb(int lsb)
{
	set_data0(lsb);
}
//------------------------------------------------------------------------------
void	PitchBend::set_msb(int msb)
{
	set_data1(msb);
}
//------------------------------------------------------------------------------
uint32_t	PitchBend::get_binary() const
{
	return get_binary_base_2();
}
} // MidiParser