#include "Aplication.h"
#include <iostream>
#include "Input.h"
#include "GLFW/glfw3.h"


Dimension::Aplication*	Dimension::Aplication::app;

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	
	Dimension::Aplication::app = this;
	window = Window::Create(title, width, height);
	window->EventsHandler(&events);
}

void Dimension::Aplication::Run() {
	//std::function<void()> test = []() {std::cout << "Testas" << std::endl;};


	//test();
	std::string text;
	while (Running) {

		m_Layers.Update();

		window->Update();
		Close();
		
		if (Input::IsKeyPressed(GLFW_KEY_R)) {
			std::cout << "R is pressed!" << std::endl;
		}

		/*events.Dispacth<KeyTypedEvent>([&text](Event& e) {
			KeyTypedEvent& event = (KeyTypedEvent&)e;
			
			text += event.GetKey();
			return true;
		});*/
		//std::cout << text << std::endl;
	}

}

Dimension::Aplication::~Aplication() {

}

void Dimension::Aplication::Close() {
	if (events.Dispacth<WindowCloseEvent>([](Event& e) {return true;})) {
		Running = false;
	}
}
