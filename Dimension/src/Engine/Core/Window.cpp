#include "Window.h"
#include "../Platforms/Window/WindowsWindow.h"
#include <memory>


namespace Dimension {
	std::shared_ptr<Window> Window::Create(const char * title, uint32_t width, uint32_t height) {
		return std::make_shared<WindowsWindow>(title, width, height);
	}
}