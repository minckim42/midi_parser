#include "midi.h"
#include "midi_out/midi_out.h"

#include <algorithm> // stable_sort
#include <thread> // sleep
#include <iostream>
#include <Windows.h>


void midi_play(const MidiParser::Midi& midi)
{
	try{
		std::vector<MidiParser::Event*> events;
		for (const MidiParser::Track& track: midi.tracks)
		{
			for (MidiParser::Event::ptr event: track.events)
			{
				events.emplace_back(event.get());
			}
		}

		// Although some events may have the same timestamp within the same track, 
		// the sequence should always be maintained.
		std::stable_sort(events.begin(), events.end(), 
			[](MidiParser::Event* a, MidiParser::Event* b)
			{
				return a->timestamp < b->timestamp; 
			}
		);

		// When calculating time with relative time, 
		// time delay may occur, 
		// so time should always be calculated with absolute time.
		std::chrono::time_point elapsed_time = system_clock::now();
		int64_t prev_timestamp = 0;
		std::chrono::microseconds delta_time_duration(0);

		for (MidiParser::Event* event: events)
		{
			int64_t delta = event->timestamp - prev_timestamp;
			std::chrono::microseconds duration = delta * delta_time_duration;
			elapsed_time += duration;
			std::this_thread::sleep_until(elapsed_time);

			if (event->get_type() == MidiParser::MetaEvent::SET_TEMPO)
			{
				delta_time_duration = midi.division.get_delta_time_duration(
					dynamic_cast<MidiParser::SetTempo*>(event)->get_quarter_note_duration()
				);
			}
			else if (event->get_category() == MidiParser::MidiEvent::MIDI)
			{
				uint32_t message = dynamic_cast<MidiParser::MidiEvent*>(event)->get_binary();
				midi_out(message);
			}
			prev_timestamp = event->timestamp;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


int main()
{
	midi_init();

	std::filesystem::path root_path(ROOT_PATH);


	std::vector<MidiParser::Midi> play_list = {
		{root_path / "sample_tamashino_refrain.mid"},
		{root_path / "sample_sonic3_boss.mid"},
	};

	for (auto& midi: play_list)
	{
		midi_play(midi);
	}
}