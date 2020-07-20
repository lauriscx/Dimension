#include "Aplication.h"
#include <iostream>

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	window = Window::Create(title, width, height);
	window->EventsHandler(&events);
}

void Dimension::Aplication::Run() {
	//std::function<void()> test = []() {std::cout << "Testas" << std::endl;};


	//test();
	std::string text;
	while (Running) {
		window->Update();
		Close();
		
		events.Dispacth<KeyTypedEvent>([&text](Event& e) {
			KeyTypedEvent& event = (KeyTypedEvent&)e;
			
			text += event.GetKey();
			return true;
		});
		std::cout << text << std::endl;
	}

}

Dimension::Aplication::~Aplication() {

}

void Dimension::Aplication::Close() {
	if (events.Dispacth<WindowCloseEvent>([](Event& e) {return true;})) {
		Running = false;
	}
}
