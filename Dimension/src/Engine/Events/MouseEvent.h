#pragma once
#include "Event.h"


namespace Dimension {
	class MausePressedEvent : public Event {
		public:
			MausePressedEvent(int key) {
				this->key = key;
				SetType(GetEventType());
			}

			static EventType GetEventType() {
				return EventType::MousePressed;
			}
			int GetKey() {
				return key;
			}
		private:
			int key;
	};

	class MauseReleaseEvent : public Event {
	public:
		MauseReleaseEvent(int key) {
			this->key = key;
			SetType(GetEventType());
		}

		static EventType GetEventType() {
			return EventType::MouseReleased;
		}
		int GetKey() {
			return key;
		}
	private:
		int key;
	};

	class MauseScrollEvent : public Event {
	public:
		MauseScrollEvent(double x, double y) {
			this->x = x;
			this->y = y;
			SetType(GetEventType());
		}

		static EventType GetEventType() {
			return EventType::MouseScroll;
		}
		double GetX() {
			return x;
		}
		double GetY() {
			return y;
		}
	private:
		double x;
		double y;
	};


	class MauseCursorEvent : public Event {
	public:
		MauseCursorEvent(double x, double y) {
			this->x = x;
			this->y = y;
			SetType(GetEventType());
		}

		static EventType GetEventType() {
			return EventType::MouseCursor;
		}
		double GetX() {
			return x;
		}
		double GetY() {
			return y;
		}
	private:
		double x;
		double y;
	};
}