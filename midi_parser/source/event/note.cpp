#include "event/note.h"
#include "util.h"
#include <sstream>
#include <iomanip>

namespace MidiParser {
/*##########################

   Note

##########################*/

/*---------------------
	constructor
---------------------*/
Note::Note(int note_number):
	note_number(note_number)
{}
//------------------------------------------------------------------------------
Note::Note(Pitch pitch, Octave octave):
	note_number(make_note_number(pitch, octave))
{}

/*---------------------
	methods
---------------------*/
Octave Note::get_octave() const
{
	return static_cast<Octave>(note_number / 12 - 2);
}
//------------------------------------------------------------------------------
Pitch Note::get_pitch() const
{
	return static_cast<Pitch>(note_number % 12);
}
//------------------------------------------------------------------------------
int Note::get_note_number() const
{
	return note_number;
}
//------------------------------------------------------------------------------
void Note::set_octave(Octave octave)
{
	Pitch pitch = get_pitch();
	note_number = make_note_number(pitch, octave);
}
//------------------------------------------------------------------------------
void Note::set_pitch(Pitch pitch)
{
	Octave octave = get_octave();
	note_number = make_note_number(pitch, octave);
}
//------------------------------------------------------------------------------
void Note::set_note(Pitch pitch, Octave octave)
{
	note_number = make_note_number(pitch, octave);
}
//------------------------------------------------------------------------------
void Note::set_note_number(int note_number)
{
	this->note_number = note_number;
}
//------------------------------------------------------------------------------
int Note::make_note_number(Pitch pitch, Octave octave)
{
	return std::clamp(
		(static_cast<int>(octave) + 2) * 12 + static_cast<int>(pitch),
		0,
		127
	);
}
//------------------------------------------------------------------------------
std::string		Note::to_string() const
{
	std::stringstream ss;
	auto octave = get_octave();
	switch (octave)
	{
		case Octave::OCTAVE_MINUS_2:
			ss << "Oc-2 - "; 
			break;
		case Octave::OCTAVE_MINUS_1:
			ss << "Oc-1 - "; 
			break;
		case Octave::OCTAVE_0:
			ss << "Oc 0 - "; 
			break;
		case Octave::OCTAVE_1:
			ss << "Oc 1 - "; 
			break;
		case Octave::OCTAVE_2:
			ss << "Oc 2 - "; 
			break;
		case Octave::OCTAVE_3:
			ss << "Oc 3 - "; 
			break;
		case Octave::OCTAVE_4:
			ss << "Oc 4 - "; 
			break;
		case Octave::OCTAVE_5:
			ss << "Oc 5 - "; 
			break;
		case Octave::OCTAVE_6:
			ss << "Oc 6 - "; 
			break;
		case Octave::OCTAVE_7:
			ss << "Oc 7 - "; 
			break;
		case Octave::OCTAVE_8:
			ss << "Oc 8 - "; 
			break;
	}

	auto pitch = get_pitch();
	switch (pitch)
	{
		case Pitch::C:
			ss << std::setw(2) << "C"; 
			break;
		case Pitch::C_SHARP:
			ss << std::setw(2) << "C#"; 
			break;
		case Pitch::D:
			ss << std::setw(2) << "D"; 
			break;
		case Pitch::D_SHARP:
			ss << std::setw(2) << "D#"; 
			break;
		case Pitch::E:
			ss << std::setw(2) << "E"; 
			break;
		case Pitch::F:
			ss << std::setw(2) << "F"; 
			break;
		case Pitch::F_SHARP:
			ss << std::setw(2) << "F#"; 
			break;
		case Pitch::G:
			ss << std::setw(2) << "G"; 
			break;
		case Pitch::G_SHARP:
			ss << std::setw(2) << "G#"; 
			break;
		case Pitch::A:
			ss << std::setw(2) << "A"; 
			break;
		case Pitch::A_SHARP:
			ss << std::setw(2) << "A#"; 
			break;
		case Pitch::B:
			ss << std::setw(2) << "B"; 
			break;
	}
	return ss.str();
}
} // MidiParser