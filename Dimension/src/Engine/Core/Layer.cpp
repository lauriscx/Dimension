#include "Layer.h"
#include "../Events/Event.h"

Dimension::Layer::Layer(const std::string & name) : m_Name(name), m_Enabled(true) {
}
void Dimension::Layer::OnAttach() {
}
void Dimension::Layer::OnDetach() {
}
void Dimension::Layer::OnUpdate() {
}
void Dimension::Layer::OnEvent(Event & event) {
}
void Dimension::Layer::Enable() {
}
void Dimension::Layer::Disable() {
}
