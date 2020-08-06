#pragma once
#include <vector>

namespace Dimension {
	class Layer;

	class LayerStack {
	public:
		LayerStack();

		void PushLayer	(Layer* layer	);
		void PushOverly	(Layer* overlay	);
		void PopLayer	(Layer* layer	);
		void PopOverlay	(Layer* overlay	);
		void Update		(				);

		~LayerStack();
	private:
		std::vector<Layer*>				m_Layers;
		std::vector<Layer*>::iterator	m_Insert;
	};
}