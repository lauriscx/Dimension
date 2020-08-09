#pragma once
#include "Event.h"

namespace Dimension {
	class KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(int Code) {
			this->Code = Code;
		}
		static EventType GetEventType() {
			return EventType::KeyReleased;
		}
		virtual EventType GetType() const override { return GetEventType(); }
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
		}
		static EventType GetEventType() {
			return EventType::Char;
		}
		virtual EventType GetType() const override { return GetEventType(); };
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
		}
		static EventType GetEventType() {
			return EventType::KeyRepeat;
		}
		virtual EventType GetType() const override { return GetEventType(); };
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