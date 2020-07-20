#pragma once
#include <string>
#include "Window.h"
#include "../Events/Event.h"
#include "../Events/Events.h"
#include "../Events/AplicationEvent.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyEvent.h"

namespace Dimension {
	class Aplication {
		public:
			Aplication(const char* title, int width, int height);
			void Run();

			static Aplication* Get() { return app; }
			std::shared_ptr<Window> GetWindow() { return window; }
			
			~Aplication();
		private:
			static Aplication* app;
			std::shared_ptr<Window> window;
			Events events;
			bool Running;

			void Close();
	};
}