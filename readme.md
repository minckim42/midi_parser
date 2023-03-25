# Midi Parser
### 미디 이벤트 단위 분석기
### 미디 포맷에 대한 이해가 필요함!

# 구성
- 샘플(윈도우즈 전용): 
	- ```CMakeLists.txt```
	- ```example.cpp```
	- ```sample*.mid```
- 미디 구문분석기: 
	- ```midi_parser/```
- 미디 출력(윈도우즈 전용): 
	- ```midi_out/```


# 샘플(윈도우즈 전용)
## 빌드 및 실행방법
- MinGW를 쓰는 경우:
	```c++
	cmake -B build -G "MinGw Makefiles"
	make -C build
	./build/midi_example.exe
	```
- 비주얼스튜디오
	- 생략

# 미디 구문분석기
## 계층 구조

- ```Midi```: 진입점
	```c++
	class Midi {
		...
		vector<Track> tracks;
		...
	}
	```

- ```Track```: 이벤트 컨테이너
	```c++
	class Track {
		...
		vector<shared_ptr<Event>> events;
		...
	}
	```

- ```Event```: 미디 메시지를 이루는 최소 단위
	- 이벤트는 크게 세 종류로 나뉘고, 한 번 더 세분화 된다
	```c++
	class Event;

	class MidiEvent: public Event;   // 연주에 관한 정보
	class NoteOff: public MidiEvent;    // 특정 음 연주 끄기
	class NoteOn: public MidiEvent;     // 특정 음 연주 켜기
	...
	
	class MetaEvent: public Event;   // 박자, 스케일 정보 등
	class SetTempo: public MetaEvent;
	class SMPTEOffset: public MetaEvent;
	...

	class SysexEvent: public Event; // 시스템 독점 이벤트
	class SysexMessages: public SysexEvent;
	class MTCQuarterFrame: public SysexEvent;
	...
	```

## 프로젝트에 추가하는 방법: CMake로 추가하기
1. ```midi_parser``` 디렉토리를 프로젝트에 복사한다.
	```
	root
	|---midi_parser
	|   |---include
	|   |---source
	|   |---CMakeLists.txt
	|
	|---CMakeLists.txt # <-- 프로젝트
	|
	```
2. 프로젝트의 ```CMakeLists.txt``` 에서 서브디렉토리 추가 및 링크
	```cmake
	...
	add_subdirectory(midi_parser)

	target_link_libraries(midi_example PRIVATE
		midi_parser
	)
	...
	```
3. 헤더 포함
	```c++
	#include "midi.h"
	```
	모든 구성요소는 네임스페이스 ```MidiParser``` 안에 있다.

	앞으로 설명중에 네임스페이스는 생략한다.

## 사용법
- (중요!)미디포멧에 대한 이해가 필요함
- ~~미디 구조를 알면 함수 이름만 보고도 쓸 수 있을 것 같음~~


1. 객체 생성
	```c++
	Midi midi(/*파일 경로*/); // 미디 객체 생성
	...
	midi.close(); // 미디 내부를 초기화한다.
	```

2. 시간 계산
	- ```delta_time```
		
		```Event```의 ```delta_time``` 멤버는 한 트랙 내에서 **직전** 이벤트로부터 흐른 시간을 나타낸다. (이벤트의 지속시간이 아님!)

		```c++
		int64_t delta_time = event->delta_time;

		Event* event = ...;
		microseconds delta_duration = ...;
		
		microseconds time = delta_time * delta_time_duration;
		```


	- ```delta_duration```

		```delta_duration```은 1 ```delta_time```의 지속시간이다. 따라서 다음과 같이 계산된다.
		
		미디에서의 시간단위는 microsecond(이하 us)이다.
		```
		time[us] = delta_duration[us] * delta_time
		```

		```delta_duration```을 구하기 위해서는 ```Division```이 필요하다.

	- ```Division```

		Midi 클래스의 멤버이고, Midi를 초기화 할 때 함께 초기화된다.
		```c++
		Midi midi(...);
		Division& division = midi.division;
		```
		
		두 가지 타입이 있다.

		- 사분음표길이의 **제수** 타입:
			```c++
			microseconds delta_time_duration = 
				division.get_delta_time_duration(quarter_note_duration)
			// delta_time_duration[us] = quarter_note_duration[us] / division
			```
			
			division 타입이 사분음표길이의 제수 타입일 때, 인자 없이 ```get_delta_time_duration()```을 호출하면 0을 반환한다.

			사분음표 길이(```quarter_note_duration```)는 ```SetTempo```가 결정한다.

		- SMPTE 타입: frame_rate와 ticks로 구성되어 있고, set_smpte로 설정한다.
			```c++
			microseconds delta_time_duration = 
				division.get_delta_time_duration();
			// delta_time_duration[us] = 1 / ( frame_rate[frame/s] * ticks[1/frame] )
			```
			
			division 타입이 SMPTE 타입일 때, ```get_delta_time_duration(quarter_note_duration)```을 호출하면 0을 반환한다.

	- 사분음표 길이(```quarter_note_duration```)
		- 사분음표 길이는 ```SetTempo```의 메서드 ```get_quarter_note_duration()```을 통해 구한다. 단위는 us이다.
			```c++
			Event* event = ...;
			auto* set_tempo = dynamic_cast<SetTempo*>(event);
			microseconds quarter_note_duration = set_tempo->get_quarter_note_duration();
			```

	- 상대시간으로 시간을 계산하면 사용자가 충분히 느낄만큼 시간 지연 현상이 발생하므로 주의한다.

		따라서 ```std::this_thread::sleep_for(...)``` 보다는 ```std::this_thread::sleep_until(...)```을 사용하는 것을 권한다.



3. 재생 방법
	- Event 처리
		- ```MidiEvent```의 경우에는 바이너리 메시지를 만든 후, midi_out의 인자로 보내면 된다.
			```c++
			uint32_t message = dynamic_cast<MidiEvent*>(event)->get_binary();
			midi_out(message);
			```
		- ```SysexEvent```는 대부분의 경우에 무시해도 된다.
		- ```MetaEvent```는 대부분의 경우, ```SetTempo```를 제외하고 대부분 무시해도 된다.

	- 시간
		- 한 프레임마다 ```delta_time```을 하나씩 더하고, 프레임마다 ```sleep_until(time + delta_time_duration)``` 하면서 재생한다.
		- event->delta_time으로 계산하는 것이 매우 귀찮기 때문에 event->timestamp를 이용하는 것을 권한다.

			```c++

			time_point t = now();
			int64_t global_timestamp = 0;

			while (true) {
				...
				for (track) {
					if (event->timestamp <= global_timestamp) {
						// 이벤트 처리
					}
				}
				...
				global_timestamp += 1;
				t += delta_duration
				sleep_until(t);
			}

			```



# 미디 출력(윈도우즈 전용)
## 프로젝트에 추가하는 방법:

소스파일과 헤더파일을 프로젝트에 복사
- 소스: midi_out/midi_out.cpp
- 헤더: midi_out/midi_out.h

## 사용법:

```c++
void midi_init();
int  midi_out(uint32_t msg);
int  midi_out(int channel, int instruction, int val0, int val1);
void midi_reset();
```

1. ```void midi_init()```
	- 미디 디바이스를 초기화 하는 함수다.
	- 이 함수를 따로 호출하지 않아도 되지만, 호출하는 것을 권장한다.
		- ```midi_out()```을 호출해도 초기화가 진행된다.
		- 초기화 시간이 짧긴 하지만, 사용자가 감지하기에 충분히 긴 시간이므로 미리 호출하는 것을 권장한다.
	- 미디 디바이스는 프로세스 종료시 자동으로 소멸한다.

2. ```int midi_out(int channel, int instruction, int val0, int val1 = 0)```
	
	```midiOutShortMsg()``` 함수의 래핑 함수이다.
	
	```midi_out(...)```함수는 호출하는 순간 리턴하는 논블록 함수이다.

	- channel: 채널 번호. 0 ~ 15 사이의 값
	- instruction: 
		```
		0 ~ 7 은 러닝스테이트로 간주되므로, 잘 알고 쓰는 것이 아니라면 권장하지 않는다.
		8 ~ 14 사이의 값을 갖는다.
			8  : Event::Type::NOTE_OFF
			9  : Event::Type::NOTE_ON
			10 : Event::Type::POLYPHONIC_KEY_PRESSURE
			11 : Event::Type::CONTROL_CHANGE
			12 : Event::Type::PROGRAM_CHANGE
			13 : Event::Type::CHANNEL_PRESSURE
			14 : Event::Type::PITCH_BEND
		val0, val1: instruction에 따라 의미가 다르다.
			NOTE_OFF: 음계, 세기
			NOTE_ON: 음계, 세기
			POLYPHONIC_KEY_PRESSURE: 음계, 세기
			CONTROL_CHANGE: 컨트롤러 번호, 값
			PROGRAM_CHANGE: 악기변경
			CHANNEL_PRESSURE: 세기
			PITCH_BEND: LSB, MSB
		- 
		```
	- 반환: midiOutShortMsg와 같다.

3. ```int midi_out(uint32_t msg)```
	- 4바이트 길이의 미디 메시지를 디바이스에 전송한다.
		```
		                      low <-- Memory --> high
		|msg                                                                 |
		|-----------------.----------------.----------------.----------------|
		|status           |value0          |value1          |0               |
		|--------.--------|                |                |                |
		|inst    |channel |                |                |                |
		|--------.--------|----------------|----------------|----------------|
		|4 bit   |4 bit   |8 bit           |8 bit           |8 bit           |
		|*.......|.*......|..**....        |....**..        |......**        |

		```
	메시지를 조립하기 귀찮으면 ```void midi_out(int channel, int instruction, int val0, int val1)```를 사용한다.



4. ```void midi_reset();```

	가끔 디바이스 자체를 리셋해야 하는 순간이 올 수도 있다. 
	
	예를들어 한 음악이 끝났는데, NOTE_OFF가 안 되었다거나, 악기가 기본값이 아닌 경우가 있을 수도 있다.