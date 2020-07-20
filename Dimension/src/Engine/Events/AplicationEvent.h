#pragma once
#include "Event.h"

namespace Dimension {
	
	class WindowCloseEvent : public Event {
		public:
			WindowCloseEvent() {
				SetType(GetEventType());
			}

			static EventType GetEventType() {
				return EventType::WindowClose;
			}
	};

	class WindowResizeEvent : public Event {
		public:
			WindowResizeEvent() {
				SetType(GetEventType());
			}
			static EventType GetEventType() {
				return EventType::WindowResize;
			}

			inline void SetWidth	(int width	) { this->width = width;	}
			inline void SetHeight	(int height	) { this->height = height;	}

			inline int GetWidth	() { return width;	}
			inline int GetHeight() { return height; }
		private:
			int width, height;
	};
}