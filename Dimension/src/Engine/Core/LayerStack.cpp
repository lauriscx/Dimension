#include "LayerStack.h"
#include "Layer.h"
#include "../Events/Events.h"

Dimension::LayerStack::LayerStack() {
	m_Insert = m_Layers.begin();
}
void Dimension::LayerStack::PushLayer(Layer * layer) {
	//Put layer in beggining  of vector.
	m_Insert = m_Layers.emplace(m_Insert, layer);
}
void Dimension::LayerStack::PushOverly(Layer * overlay) {
	//Put layer to end of vector.
	m_Layers.emplace_back(overlay);
}
void Dimension::LayerStack::PopLayer(Layer * layer) {
	//Remove from beggining of vector;
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end()) {
		m_Layers.erase(it);
		m_Insert--;
	}
}
void Dimension::LayerStack::PopOverlay(Layer * overlay) {
	//Remove from end of vector;
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end()) {
		m_Layers.erase(it);
	}
}
void Dimension::LayerStack::Update() {
	for (Layer* layer : m_Layers) {
		layer->OnUpdate();
	}
}

void Dimension::LayerStack::OnEvent(Events & events) {
	for (Layer* layer : m_Layers) {
		if (layer)
			layer->OnEvent(events);
	}
}

Dimension::LayerStack::~LayerStack() {
	for (Layer* layer : m_Layers) {
		if (layer)
			delete layer;
	}
}