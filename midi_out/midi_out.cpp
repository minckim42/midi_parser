#include <windows.h>
#include <cstdint>
#include <bit>

struct MidiDevice
{
	HMIDIOUT handle = nullptr;
	MidiDevice()
	{
		midiOutOpen(&handle, MIDI_MAPPER, 0, 0, CALLBACK_NULL);
	}
	~MidiDevice()
	{
		midiOutReset(handle);
		midiOutClose(handle);
	}
};

static 
HMIDIOUT midi_device()
{
	static MidiDevice device;
	return device.handle;
}

void midi_init()
{
	midi_device();
}

void midi_out(uint32_t msg)
{
	static HMIDIOUT handle = midi_device();
	midiOutShortMsg(handle, msg);
}

void midi_out(int channel, int instruction, int val0, int val1)
{
	if constexpr (std::endian::native == std::endian::little)
	{
		midiOutShortMsg(
			midi_device(),
			(channel & 0xf)     <<  0 |
			(instruction & 0xf) <<  4 |
			(val0 & 0xff)       <<  8 |
			(val1 & 0xff)       << 16 
		);
	}
	else // constexpr (std::endian::native == std::endian::big)
	{
		midiOutShortMsg(
			midi_device(),
			(channel & 0xf)     << 28 |
			(instruction & 0xf) << 24 |
			(val0 & 0xff)       << 18 |
			(val1 & 0xff)       <<  8 
		);
	}
}