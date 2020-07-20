#include <functional>
#include <vector>
#include <string>
#include "Event.h"
#include <iostream>
#include <string>

namespace Dimension {
	class Events {
	public:
		void AddEvent(Event event) { events.push_back(event); }

		template <typename T, typename F>
		bool Dispacth(F function) {
			int i = 0;
			for (Event e : events) {
				if (e.GetType() == T::GetEventType()) {
					//Absorb event by function.
					if (function(e)) {
						events.erase(events.begin() + i);
						return true;
					}
					i++;
					return true;
				}
			}
			return false;
		}

	private:
		std::vector<Event> events;
	};
}