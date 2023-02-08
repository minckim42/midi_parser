#pragma once
#include "common.h"
#include "event.h"
#include "note.h"
#include "instrument.h"
#include "controller.h"
#include <string>

namespace MidiParser {
/*##########################

	MidiEvent

##########################*/
class MidiEvent : public Event
{
public:
	/*---------------------
		enumeration
	---------------------*/
	enum Type
	{
		NOTE_OFF = 8,
		NOTE_ON = 9,
		POLYPHONIC_KEY_PRESSURE = 10,
		CONTROL_CHANGE = 11,
		PROGRAM_CHANGE = 12,
		CHANNEL_PRESSURE = 13,
		PITCH_BEND = 14,
	};

	/*---------------------
		methods
	---------------------*/
	int				get_channel() const;
	
	virtual
	int				get_status() const override = 0;
	
	virtual
	std::ostream&	str(std::ostream& os) const override;
	
	Event::Type 	get_event_type() const override;
	
	virtual
	Type			get_midi_type() const = 0;
	
	void			set_channel(int channel);
	
	virtual
	uint32_t		get_binary() const = 0;
	

	static
	std::shared_ptr<Event>	create(
		uint64_t				delta_time,
		int						status,
		const byte*&			begin,
		const byte*				end
	);

	/*---------------------
		members
	---------------------*/
	byte data[2];


protected:
	/*---------------------
		members
	---------------------*/
	byte channel = 0;

	/*---------------------
		constructor
	---------------------*/
	MidiEvent(uint64_t delta_time, int channel);
	MidiEvent() = default;
	MidiEvent(const MidiEvent &) = default;
	MidiEvent(MidiEvent &&) noexcept = default;
	MidiEvent &operator=(const MidiEvent &) = default;
	MidiEvent &operator=(MidiEvent &&) noexcept = default;
	~MidiEvent() = default;

	/*---------------------
		methods
	---------------------*/
	Note		get_note_base() const;
	void		set_note_base(Note note);
	void		set_data0(int data0);
	void		set_data1(int data1);
	void		set_data(int data0, int data1);
	inline		

	uint32_t	get_binary_base_1() const
	{
		uint32_t result = 0;
		byte* ptr = reinterpret_cast<byte*>(&result);
		ptr[0] = get_status();
		ptr[1] = data[0];
		return result;
	}

	inline		
	uint32_t	get_binary_base_2() const
	{
		uint32_t result = 0;
		byte* ptr = reinterpret_cast<byte*>(&result);
		ptr[0] = get_status();
		ptr[1] = data[0];
		ptr[2] = data[1];
		return result;
	}
};




/*##########################

	NoteOff

##########################*/
class NoteOff final : public MidiEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	NoteOff() = default;
	NoteOff(
		uint64_t	delta_time,
		int			channel = 0,
		Note		note = Note(Pitch::C, Octave::OCTAVE_3),
		int			velocity = 0x40);

	/*---------------------
		methods
	---------------------*/
	int					get_status() const override;
	Type				get_midi_type() const override;
	std::ostream&		str(std::ostream& os) const override;
	Note				get_note() const;
	int					get_velocity() const;
	void				set_note(Note note);
	void				set_note(int note_number);
	void				set_note(Pitch pitch, Octave octave);
	void				set_velocity(int velocity);
	uint32_t	get_binary() const override;
};




/*##########################

	NoteOn

##########################*/
class NoteOn final : public MidiEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	NoteOn() = default;
	NoteOn(
		uint64_t	delta_time,
		int			channel = 0,
		Note		note = Note(Pitch::C, Octave::OCTAVE_3),
		int			velocity = 0x40);

	/*---------------------
		methods
	---------------------*/
	int		get_status() const override;
	Type	get_midi_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	Note	get_note() const;
	int		get_velocity() const;
	void	set_note(Note note);
	void	set_note(int note_number);
	void	set_note(Pitch pitch, Octave octave);
	void	set_velocity(int velocity);
	uint32_t	get_binary() const override;
};




/*##########################

	PolyphonicKeyPressure

##########################*/
class PolyphonicKeyPressure final : public MidiEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	PolyphonicKeyPressure() = default;
	PolyphonicKeyPressure(
		uint64_t	delta_time,
		int			channel = 0,
		Note		note = Note(Pitch::C, Octave::OCTAVE_3),
		int			pressure = 0x40
	);

	/*---------------------
		methods
	---------------------*/
	int		get_status() const override;
	Type	get_midi_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	Note	get_note() const;
	int		get_pressure() const;
	void	set_note(Note note);
	void	set_note(int note_number);
	void	set_note(Pitch pitch, Octave octave);
	void	set_pressure(int pressure);
	uint32_t	get_binary() const override;
};




/*##########################

	ControlChange

##########################*/
class ControlChange final : public MidiEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	ControlChange() = default;
	ControlChange(
		uint64_t	delta_time,
		int			channel = 0,
		Controller	controller = Controller::BANK_SELECT_DETAIL,
		int			value = 0x40
	);

	/*---------------------
		methods
	---------------------*/
	int			get_status() const override;
	Type		get_midi_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	Controller	get_controller() const;
	int			get_value() const;
	void		set_controller(Controller controller);
	void		set_value(int value);
	uint32_t	get_binary() const override;
};




/*##########################

	ProgramChange

##########################*/
class ProgramChange final : public MidiEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	ProgramChange() = default;
	ProgramChange(
		uint64_t	delta_time,
		int			channel = 0,
		Instrument	instrument = Instrument::ACOUSTIC_GRAND
	);

	/*---------------------
		methods
	---------------------*/
	int			get_status() const override;
	Type		get_midi_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	Instrument	get_instrument() const;
	void		set_instrument(Instrument instrument);
	uint32_t	get_binary() const override;
};




/*##########################

	ChannelPressure

##########################*/
class ChannelPressure final : public MidiEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	ChannelPressure() = default;
	ChannelPressure(uint64_t delta_time, int channel = 0, int pressure = 0x40);

	/*---------------------
		methods
	---------------------*/
	int get_status() const override;
	Type get_midi_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	int get_pressure() const;
	void set_pressure(int pressure);
	uint32_t	get_binary() const override;
};




/*##########################

	PitchBend

##########################*/
class PitchBend final : public MidiEvent
{
public:
	/*---------------------
		constructors
	---------------------*/
	PitchBend() = default;
	PitchBend(uint64_t delta_time, int channel = 0, int lsb = 0x40, int msb = 0x40);

	/*---------------------
		methods
	---------------------*/
	int get_status() const override;
	Type get_midi_type() const override;
	std::ostream&	str(std::ostream& os) const override;
	int get_lsb() const;
	int get_msb() const;
	void set_lsb(int lsb);
	void set_msb(int msb);
	uint32_t	get_binary() const override;
};
} // MidiParser