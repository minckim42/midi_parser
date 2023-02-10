#include "midi.h"
#include <algorithm>
#include <windows.h>
#include <thread>
#include <iostream>


using namespace MidiParser;
using namespace std;

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

HMIDIOUT midi_device()
{
	static MidiDevice device;
	return device.handle;
}


int main()
{
	midi_device();
	try{
		Midi midi("refrain.mid");

		std::vector<Event*> events;
		for (Track& track: midi.tracks)
		{
			for (Event::ptr event: track.events)
			{
				events.emplace_back(event.get());
			}
		}

		stable_sort(events.begin(), events.end(), 
			[](Event* a, Event* b)
			{
				return a->timestamp < b->timestamp; 
			}
		);

		chrono::time_point prev = system_clock::now();
		int64_t prev_timestamp = 0;
		microseconds delta_time_duration(0);
		for (Event* event: events)
		{
			int64_t delta = event->timestamp - prev_timestamp;
			prev_timestamp = event->timestamp;
			microseconds duration = delta * delta_time_duration;
			prev += duration;
			this_thread::sleep_until(prev);
			if (event->get_event_type() == MidiEvent::META)
			{
				auto* meta = dynamic_cast<MetaEvent*>(event);
				if (meta->get_meta_type() == MetaEvent::SET_TEMPO)
				{
					delta_time_duration = midi.division->get_delta_time_duration(
						dynamic_cast<SetTempo*>(meta)->get_quarter_note_duration()
					);
				}
			}
			else if (event->get_event_type() == MidiEvent::MIDI)
			{
				uint32_t message = dynamic_cast<MidiEvent*>(event)->get_binary();
				midiOutShortMsg(midi_device(), message);
			}
		}
	}
	catch (exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}