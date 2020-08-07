#include "Layer.h"
#include "../Events/Events.h"
#include <iostream>
#include <string>

Dimension::Layer::Layer(const std::string & name) : m_Name(name), m_Enabled(true) {
}
void Dimension::Layer::OnAttach() {
	std::cout << (m_Name + " attached") << std::endl;
}
void Dimension::Layer::OnDetach() {
	std::cout << (m_Name + " Detached") << std::endl;
}
void Dimension::Layer::OnUpdate() {
}
void Dimension::Layer::OnEvent(Events & events) {
	std::cout << (m_Name + " callback event") << std::endl;
}
void Dimension::Layer::Enable() {
}
void Dimension::Layer::Disable() {
}

Dimension::Layer::~Layer() {
}

void Dimension::Layer::SetEnable(bool Enable) {
	m_Enabled = Enable;
}
