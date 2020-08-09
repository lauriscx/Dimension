#pragma once
#include "Event.h"


namespace Dimension {
	class MausePressedEvent : public Event {
		public:
			MausePressedEvent(int key) {
				this->key = key;
			}
			static EventType GetEventType() {
				return EventType::MousePressed;
			}
			virtual EventType GetType() const override { return GetEventType(); };
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
		}
		static EventType GetEventType() {
			return EventType::MouseReleased;
		}
		virtual EventType GetType() const override { return GetEventType(); };
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
		}

		static EventType GetEventType() {
			return EventType::MouseScroll;
		}
		virtual EventType GetType() const override { return GetEventType(); };
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
		}

		static EventType GetEventType() {
			return EventType::MouseCursor;
		}

		virtual EventType GetType() const override { return GetEventType(); };
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