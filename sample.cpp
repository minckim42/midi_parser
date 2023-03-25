#include "midi.h"
#include "midi_out/midi_out.h"

#include <algorithm> // stable_sort
#include <thread> // sleep
#include <iostream>
#include <Windows.h>

void midi_play(const MidiParser::Midi& midi)
{
	std::vector<int> indices(midi.tracks.size(), 0);
	int64_t timestamp = 0;
	MidiParser::Microseconds delta_time_duration(0);
	bool playing = true;
	MidiParser::Timepoint time = MidiParser::Clock::now();

	while (playing)
	{
		playing = false;

		for (int track_idx = 0; track_idx < midi.tracks.size(); ++track_idx)
		{
			const MidiParser::Track& track = midi.tracks[track_idx];
			int& event_idx = indices[track_idx];
			while (event_idx < track.events.size())
			{
				const MidiParser::Event* event = track.events[event_idx].get();
				if (event->timestamp > timestamp) break;

				if (event->get_category() == MidiParser::MidiEvent::MIDI)
				{
					midi_out(dynamic_cast<const MidiParser::MidiEvent*>(event)->get_binary());
					std::cout << event->str() << std::endl;
				}
				else if (event->get_type() == MidiParser::MetaEvent::SET_TEMPO)
				{
					delta_time_duration = midi.division.get_delta_time_duration(
						dynamic_cast<const MidiParser::SetTempo*>(event)->get_quarter_note_duration()
					);
					std::cout << event->str() << std::endl;
				}
				++event_idx;
			}
			if (event_idx < track.events.size()) playing = true;
		}
		++timestamp;

		// Time MUST be calculated by absolute time.
		// Relative time MAY cause time delay;
		std::this_thread::sleep_until(time += delta_time_duration);
	}

}

int main()
{
	midi_init();

	std::filesystem::path root_path(ROOT_PATH);

	std::vector<MidiParser::Midi> play_list = {
		{root_path / "sample_sonic3_boss.mid"},
		{root_path / "sample_tamashino_refrain.mid"},
		{root_path / "these.mid"},
	};

	for (auto& midi: play_list)
	{
		midi.save_str();
		midi_play(midi);
		midi_reset();
	}
}