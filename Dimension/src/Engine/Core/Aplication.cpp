#include "Aplication.h"
#include <iostream>
#include "Input.h"
#include "GLFW/glfw3.h"
#include <functional>
#include "Layer.h"

Dimension::Aplication*	Dimension::Aplication::app;

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	
	Dimension::Aplication::app = this;
	window = Window::Create(title, width, height);
	window->EventsHandler(&events);
	events.OnEvent(std::bind(&LayerStack::OnEvent, &m_Layers, std::placeholders::_1));
}

void Dimension::Aplication::Run() {

	Layer * layer = new Layer("Test");
	m_Layers.PushLayer(layer);

	//test();
	std::string text;
	while (Running) {

		m_Layers.Update();

		window->Update();
		Close();
		
		if (Input::IsKeyPressed(GLFW_KEY_R)) {
			std::cout << "R is pressed!" << std::endl;
			m_Layers.PopLayer(layer);
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
