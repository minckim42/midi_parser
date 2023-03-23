# Midi Parser
### 미디 이벤트 단위 분석기
### 미디 포맷에 대한 이해가 필요함!

# 구성
- 예제(윈도우즈 전용): 
	- CMakeLists.txt
	- example.cpp, 
	- sample*.mid
- 미디 구문분석기: 
	- midi_parser/
- 미디 출력(윈도우즈 전용): 
	- midi_out/


# 예제(윈도우즈 전용)
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

## 사용법
- (중요!)미디포멧에 대한 이해가 필요하다.
- ~~미디 구조를 알면 함수 이름만 보고도 쓸 수 있을 것 같음~~

1. 헤더 포함
	```c++
	#include "midi.h"
	```
	모든 구성요소는 네임스페이스 ```MidiParser``` 안에 있다.

	앞으로 설명중에 네임스페이스는 생략한다.

2. 객체 생성
	```c++
	Midi midi(/*파일 경로*/); // 미디 객체 생성
	...
	midi.close(); // 미디 내부를 초기화한다.
	```

3. 클래스 구조
	- ```Midi```: ```Track``` 배열을 갖고 있다
		```c++
		class Midi {
			...
			vector<Track> tracks;
			...
		}
		```

	- ```Track```: ```Event``` 배열을 갖고 있다
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

		class MidiEvent: public Event;  // 연주 정보
		class NoteOff: public MidiEvent;
		class NoteOn: public MidiEvent;
		...
		
		class MetaEvent: public Event;  // 박자, 스케일 정보 등
		class SetTempo: public MetaEvent;
		class SMPTEOffset: public MetaEvent;
		...

		class SysexEvent: public Event; // 시스템 독점 이벤트
		class SysexMessages: public SysexEvent;
		class MTCQuarterFrame: public SysexEvent;
		...
		```

4. 재생 방법
	- 시간 계산
		- ```delta_time```
			- ```Event```의 ```delta_time``` 멤버는 트랙의 **직전** 이벤트로부터 흐른 시간을 나타낸다.
				```c++
				Event* event = ...;
				int64_t delta_time = event->delta_time;
				microseconds time = delta_time * delta_time_duration; 
				//time(us) = delta_time * delta_time_duration(us)
				```
			- 여기서 ```delta_time_duration```은 ```Midi``` 클래스의 멤버 ```division```을 통해서 구한다.
				```c++
				Midi midi(...);
				Division& division = midi.division;
				```
		- Division
			- ```Division```은 시간 단위에 대한 정보다. 타입은 둘 중 하나다.
				- 사분음표 길이(quarter_note_duration) 타입:
					```c++
					microseconds delta_time_duration = 
						division.get_delta_time_duration(quarter_note_duration)
					// delta_time_duration(us) = quarter_note_duration(us) / division
					```
				- SMPTE 타입: frame_rate와 ticks로 구성되어 있고, set_smpte로 설정가능하다.
					```c++
					microseconds delta_time_duration = 
						division.get_delta_time_duration();
					// delta_time_duration(us) = 1 / ( frame_rate(frame/s) * ticks(1/frame) )
					```
			- division 타입이 사분음표 길이 타입일 때 인자 없이 ```get_delta_time_duration()```을 호출하면 0을 반환한다.
			- 반대로 division 타입이 SMPTE 타입일 때 ```get_delta_time_duration(quarter_note_duration)```을 호출하면 0을 반환한다.
			- 사분음표 길이는 ```SetTempo```의 메서드 ```get_quarter_note_duration()```을 통해 구한다.
				```c++
				Event* = ...;
				auto* set_tempo = dynamic_cast<SetTempo*>(event);
				microseconds quarter_note_duration = set_tempo->get_quarter_note_duration();
				```
		- 상대시간으로 시간을 계산하면 사용자가 충분히 느낄만큼 시간 지연 현상이 발생하므로 주의한다.

	- Event 처리
		- ```MidiEvent```의 경우에는 바이너리 메시지를 만든 후, midi_out의 인자로 보내면 된다.
			```c++
			uint32_t message = dynamic_cast<MidiEvent*>(event)->get_binary();
			midi_out(message);
			```
		- ```SysexEvent```는 무시해도 된다.
		- ```MetaEvent```는 대부분의 경우, ```SetTempo```만 처리하면 된다.




# 미디 출력(윈도우즈 전용)
## 프로젝트에 추가하는 방법:

소스파일과 헤더파일을 프로젝트에 복사
- 소스: midi_out/midi_out.cpp
- 헤더: midi_out/midi_out.h

## 사용법:

```c++
void midi_init();
void midi_out(uint32_t msg);
void midi_out(int channel, int instruction, int val0, int val1);
```

1. ```void midi_init()```
	- 미디 디바이스를 초기화 하는 함수다.
	- 이 함수를 따로 호출하지 않아도 되지만, 호출하는 것을 권장한다.
		- midi_out()을 호출해도 초기화가 진행된다.
		- 초기화 시간이 짧긴 하지만, 사용자가 감지하기에 충분히 긴 시간이므로 미리 호출하는 것을 권장한다.
	- 미디 디바이스는 프로세스 종료시 자동으로 소멸한다.

2. ```void midi_out(uint32_t msg)```
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
	- 메시지를 조립하기 귀찮으면 아래 함수를 사용한다.

3. ```void midi_out(int channel, int instruction, int val0, int val1)```

- ```midi_out(...)```함수는 호출하는 순간 리턴하는 논블록 함수이다.

