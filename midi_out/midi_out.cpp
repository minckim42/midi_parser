#include <windows.h>
#include <cstdint>
#include <bit>

struct MidiDevice
{
	HMIDIOUT handle = nullptr;
	MidiDevice()
	{
		init();
	}
	~MidiDevice()
	{
		close();
	}
	void init()
	{
		midiOutOpen(&handle, MIDI_MAPPER, 0, 0, CALLBACK_NULL);
	}
	void close()
	{
		midiOutReset(handle);
		midiOutClose(handle);
		handle = nullptr;
	}
	void reset()
	{
		close();
		init();
	}
	operator HMIDIOUT()
	{
		return handle;
	}
};

static 
MidiDevice& midi_device()
{
	static MidiDevice device;
	return device;
}

void midi_init()
{
	midi_device();
}

void midi_reset()
{
	midi_device().reset();
}

int midi_out(uint32_t msg)
{
	return midiOutShortMsg(midi_device(), msg);
}

int midi_out(int channel, int instruction, int val0, int val1)
{
	if constexpr (std::endian::native == std::endian::little)
	{
		return midiOutShortMsg(
			midi_device(),
			(channel & 0xf)     <<  0 |
			(instruction & 0xf) <<  4 |
			(val0 & 0xff)       <<  8 |
			(val1 & 0xff)       << 16 
		);
	}
	else // constexpr (std::endian::native == std::endian::big)
	{
		return midiOutShortMsg(
			midi_device(),
			(channel & 0xf)     << 28 |
			(instruction & 0xf) << 24 |
			(val0 & 0xff)       << 18 |
			(val1 & 0xff)       <<  8 
		);
	}
}