#pragma once
#include <vector>
#include <string>

namespace Dimension {
	enum class EventType {
		None,
		WindowResize,
		WindowClose,
		KeyPressed,
		KeyReleased,
		KeyRepeat,
		Char,
		MousePressed,
		MouseReleased,
		MouseScroll,
		MouseCursor
	};

	class Event {
		public:
			EventType GetType() {
				return type;
			}
			void  SetType(EventType type) {
				this->type = type;
			}
		private:
			EventType type;
	};
}