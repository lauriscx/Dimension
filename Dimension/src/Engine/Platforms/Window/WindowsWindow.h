#pragma once
#include "../../Core/Window.h"

struct GLFWwindow;

namespace Dimension {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const char* title, const uint32_t width, const uint32_t height);
		// Inherited via Window
		virtual uint32_t	GetWidth		()				override;
		virtual uint32_t	GetHeight		()				override;
		virtual float		GetAspectRation	()				override;
		virtual void		Update			()				override;
		virtual void		EventsHandler	(Events* event)	override;
		virtual void *		Context			()				override;

		virtual ~WindowsWindow();
	private:
		uint32_t	width, height;
		std::string title;
		GLFWwindow* window;
	};
}