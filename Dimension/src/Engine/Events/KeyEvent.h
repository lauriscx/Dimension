#pragma once
#include "Event.h"

namespace Dimension {
	class KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(int Code) {
			this->Code = Code;
			SetType(GetEventType());
		}

		static EventType GetEventType() {
			return EventType::KeyReleased;
		}
		int GetKey() {
			return Code;
		}
	private:
		int Code;
	};

	class KeyTypedEvent : public Event {
	public:
		KeyTypedEvent(char Code) {
			this->Code = Code;
			SetType(GetEventType());
		}

		static EventType GetEventType() {
			return EventType::Char;
		}
		char GetKey() {
			return Code;
		}
	private:
		char Code;
	};

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int Code, int Repeat) {
			this->Code = Code;
			this->Repeat = Repeat;
			SetType(GetEventType());
		}

		static EventType GetEventType() {
			return EventType::KeyRepeat;
		}
		int GetKey() {
			return Code;
		}
		int GetRepeat() {
			return Repeat;
		}
	private:
		int Code;
		int Repeat;
	};
}