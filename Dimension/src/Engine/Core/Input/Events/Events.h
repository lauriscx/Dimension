#include <functional>
#include <vector>
#include <string>
#include "Event.h"
#include <iostream>
#include <string>
#include "Error.h"
#include "AplicationEvent.h"

//Event system if fucked up.

namespace Dimension {
	class Events {
	public:
		void AddEvent(Event* event) {
			events.push_back(event);
			/*
			Dispacth<WindowResizeEvent>([](WindowResizeEvent* e) {
				//std::string er = e.GetErrorReport();
				std::cout << "Window width: " << e->GetWidth() << std::endl;
				return true;
			});

			Dispacth<Error>([](Error* e) {
				//std::string er = e.GetErrorReport();
				std::cout << "Error " << e->GetErrorReport() << std::endl;
				return true;
			});
			Dispacth<WindowCloseEvent>([](WindowCloseEvent* e) {
				//std::string er = e.GetErrorReport();
				std::cout << "Close " << std::endl;
				return true;
			});*/
			//callBack(*this);
		}
		void OnEvent(std::function<void(Events&)> function) { callBack = function; }

		template <typename T, typename F>
		bool Dispacth(const F& function) {
			int i = 0;
			for (Event* e : events) {
				if (e->GetType() == T::GetEventType()) {
					T* event = static_cast<T*>(e);
					//Absorb event by function.
					if (function(event)) {//Cast from parent to childs class
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
		std::vector<Event*> events;
		std::function<void(Events&)> callBack;
	};
}