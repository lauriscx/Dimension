#pragma once

namespace Dimension {
	enum class EventType {
		None,
		Error,
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
			virtual ~Event() {};
			virtual EventType GetType() const { return EventType::None; };
	};
}