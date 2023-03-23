#pragma once
#include "common.h"
#include <string>

namespace MidiParser {
enum class Octave
{
	OCTAVE_MINUS_2 = -2,
	OCTAVE_MINUS_1 = -1,
	OCTAVE_0 = 0,
	OCTAVE_1 = 1,
	OCTAVE_2 = 2,
	OCTAVE_3 = 3,
	OCTAVE_4 = 4,
	OCTAVE_5 = 5,
	OCTAVE_6 = 6,
	OCTAVE_7 = 7,
	OCTAVE_8 = 8,
};
enum class Pitch
{
	C = 0,
	C_SHARP = 1,
	D = 2,
	D_SHARP = 3,
	E = 4,
	F = 5,
	F_SHARP = 6,
	G = 7,
	G_SHARP = 8,
	A = 9,
	A_SHARP = 10,
	B = 11,
};

/*##########################

	Note

##########################*/
class Note
{
public:
	/*---------------------
		constructor
	---------------------*/
	Note(int note_number);
	Note(Pitch pitch = Pitch::C, Octave octave = Octave::OCTAVE_3);

	/*---------------------
		methods
	---------------------*/
	static 
	int				make_note_number(Pitch pitch, Octave Octave);
	
	Octave			get_octave() const;
	Pitch			get_pitch() const;
	int				get_note_number() const;
	void			set_octave(Octave octave);
	void			set_pitch(Pitch pitch);
	void			set_note(Pitch pitch, Octave Octave);
	void			set_note_number(int note_number);
	std::string		to_string() const;


private:
	/*---------------------
		member
	---------------------*/
	byte note_number = 60;
};
} // MidiParser