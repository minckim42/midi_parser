#include "event/controller.h"
#include "util.h"

namespace MidiParser {
/*##########################

   Controller

##########################*/
Controller::Controller(Controller::Type type)
{
	set_type(type);
}
//------------------------------------------------------------------------------
Controller::Controller(int type)
{
	set_type(static_cast<Type>(type));
}
//------------------------------------------------------------------------------
std::string Controller::search_type_name(Controller::Type type)
{
	switch (type)
	{
	case 0:
		return "Bank Select (Detail)";
	case 1:
		return "Modulation Wheel";
	case 2:
		return "Breath Controller";
	case 4:
		return "Foot Controller";
	case 5:
		return "Portamento Time";
	case 6:
		return "Data Entry (used with RPNs/NRPNs)";
	case 7:
		return "Channel Volume";
	case 8:
		return "Balance";
	case 10:
		return "Pan";
	case 11:
		return "Expression Controller";
	case 12:
		return "Effect Control 1";
	case 13:
		return "Effect Control 2";
	case 16:
		return "Gen Purpose Controller 1";
	case 17:
		return "Gen Purpose Controller 2";
	case 18:
		return "Gen Purpose Controller 3";
	case 19:
		return "Gen Purpose Controller 4";
	case 32:
		return "Bank Select";
	case 33:
		return "Modulation Wheel";
	case 34:
		return "Breath Controller";
	case 36:
		return "Foot Controller";
	case 37:
		return "Portamento Time";
	case 38:
		return "Data Entry";
	case 39:
		return "Channel Volume";
	case 40:
		return "Balance";
	case 42:
		return "Pan";
	case 43:
		return "Expression Controller";
	case 44:
		return "Effect Control 1";
	case 45:
		return "Effect Control 2";
	case 48:
		return "General Purpose Controller 1";
	case 49:
		return "General Purpose Controller 2";
	case 50:
		return "General Purpose Controller 3";
	case 51:
		return "General Purpose Controller 4";
	case 64:
		return "Sustain On/Off";
	case 65:
		return "Portamento On/Off";
	case 66:
		return "Sostenuto On/Off";
	case 67:
		return "Soft Pedal On/Off";
	case 68:
		return "Legato On/Off";
	case 69:
		return "Hold 2 On/Off";
	case 70:
		return "Sound Controller 1   (TG: Sound Variation;   FX: Exciter On/Off)";
	case 71:
		return "Sound Controller 2   (TG: Harmonic Content;   FX: Compressor On/Off)";
	case 72:
		return "Sound Controller 3   (TG: Release Time;   FX: Distortion On/Off)";
	case 73:
		return "Sound Controller 4   (TG: Attack Time;   FX: EQ On/Off)";
	case 74:
		return "Sound Controller 5   (TG: Brightness;   FX: Expander On/Off)";
	case 75:
		return "Sound Controller 6   (TG: Decay Time;   FX: Reverb On/Off)";
	case 76:
		return "Sound Controller 7   (TG: Vibrato Rate;   FX: Delay On/Off)";
	case 77:
		return "Sound Controller 8   (TG: Vibrato Depth;   FX: Pitch Transpose On/Off)";
	case 78:
		return "Sound Controller 9   (TG: Vibrato Delay;   FX: Flange/Chorus On/Off)";
	case 79:
		return "Sound Controller 10   (TG: Undefined;   FX: Special Effects On/Off)";
	case 80:
		return "General Purpose Controller 5";
	case 81:
		return "General Purpose Controller 6";
	case 82:
		return "General Purpose Controller 7";
	case 83:
		return "General Purpose Controller 8";
	case 84:
		return "Portamento Control (PTC)   (0vvvvvvv is the source Note number)   (Detail)";
	case 88:
		return "High Resolution Velocity Prefix";
	case 91:
		return "Effects 1 Depth (Reverb Send Level)";
	case 92:
		return "Effects 2 Depth (Tremelo Depth)";
	case 93:
		return "Effects 3 Depth (Chorus Send Level)";
	case 94:
		return "Effects 4 Depth (Celeste Depth)";
	case 95:
		return "Effects 5 Depth (Phaser Depth)";
	case 96:
		return "Data Increment";
	case 97:
		return "Data Decrement";
	case 98:
		return "Non Registered Parameter Number (LSB)";
	case 99:
		return "Non Registered Parameter Number (MSB)";
	case 100:
		return "Registered Parameter Number (LSB)";
	case 101:
		return "Registered Parameter Number (MSB)";
	case 120:
		return "All Sound Off";
	case 121:
		return "Reset All Controllers";
	case 122:
		return "Local Control On/Off";
	case 123:
		return "All Notes Off";
	case 124:
		return "Omni Mode Off (also causes ANO)";
	case 125:
		return "Omni Mode On (also causes ANO)";
	case 126:
		return "Mono Mode On (Poly Off; also causes ANO)";
	case 127:
		return "Poly Mode On (Mono Off; also causes ANO)";
	default:
		return "Undefined";
	}
}
//------------------------------------------------------------------------------
std::string Controller::get_type_name() const
{
	return search_type_name(type);
}
//------------------------------------------------------------------------------
Controller::Type Controller::get_type() const
{
	return type;
}
//------------------------------------------------------------------------------
void Controller::set_type(Controller::Type type)
{
	this->type = static_cast<Type>(std::clamp(static_cast<int>(type), 0, 127));
}
} // MidiParser