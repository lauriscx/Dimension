#pragma once
#include "Event.h"

namespace Dimension {
	
	class WindowCloseEvent : public Event {
		public:

			static EventType GetEventType() {
				return EventType::WindowClose;
			}
			virtual EventType GetType() const override { return GetEventType(); }
	};

	class WindowResizeEvent : public Event {
		public:
			static EventType GetEventType() {
				return EventType::WindowResize;
			}
			virtual EventType GetType() const override { return GetEventType(); }

			inline void SetWidth	(int width	) { this->width = width;	}
			inline void SetHeight	(int height	) { this->height = height;	}

			inline int GetWidth	() { return width;	}
			inline int GetHeight() { return height; }
		private:
			int width, height;
	};
}