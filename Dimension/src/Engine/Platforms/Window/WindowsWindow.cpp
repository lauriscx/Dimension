#include "WindowsWindow.h"
#include "GLFW/glfw3.h"
#include "../../Core/Input/Events/Events.h"
#include "../../Core/Input/Events/AplicationEvent.h"
#include "../../Core/Input/Events/MouseEvent.h"
#include "../../Core/Input/Events/KeyEvent.h"
#include "../../Core/Input/Events/Error.h"
#include <iostream>
#include <string>


bool Dimension::WindowsWindow::errorOccour = false;
Dimension::Error* Dimension::WindowsWindow::s_error = nullptr;

Dimension::WindowsWindow::WindowsWindow	(const char * title, const uint32_t width, const uint32_t height) {
	//Create error event on callback
	glfwSetErrorCallback([](int error, const char* description) {
		WindowsWindow::s_error = new Error();
		WindowsWindow::s_error->SetCode(error);
		WindowsWindow::s_error->SetSubSystem("GLFW");
		WindowsWindow::s_error->SetDescription(description);
		WindowsWindow::errorOccour = true;
	});

	/* Initialize the library */
	if (!glfwInit())
		return;
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, title, NULL, NULL);

	if (!window) {
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	this->height = height;
	this->width = width;
	this->title = title;
	this->fullScreen = false;
}

uint32_t	Dimension::WindowsWindow::GetWidth			() {
	return width;
}
uint32_t	Dimension::WindowsWindow::GetHeight			() {
	return height;
}
float		Dimension::WindowsWindow::GetAspectRation	() {
	return (float)width / (float)height;
}
void		Dimension::WindowsWindow::Update			() {
	if (errorOccour == true) {
		errorOccour = false;
		if (events != nullptr) {
			if (WindowsWindow::s_error != nullptr) {
				events->AddEvent(WindowsWindow::s_error);
			}
		}
	}
	/* Poll for and process events */
	glfwPollEvents();

	/* Swap front and back buffers */
	glfwSwapBuffers(window);
}
void Dimension::WindowsWindow::SetVsync(bool vsync) {
	glfwSwapInterval(vsync);
}
void Dimension::WindowsWindow::SetFullScreen(bool FullScreen)
{
	if (fullScreen == FullScreen)
		return;

	if (FullScreen)
	{
		// backup window position and window size
		glfwGetWindowPos(window, &x, &y);
		glfwGetWindowSize(window, &width, &height);

		// get resolution of monitor
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		// switch to full screen
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
	}
	else
	{
		// restore last window size and position
		glfwSetWindowMonitor(window, nullptr, x, y, width, height, 0);
	}
	fullScreen = FullScreen;
}
void		Dimension::WindowsWindow::EventsHandler		(Events* event) {
	this->events = event;
	//Pass events class to glfw for events input.
	glfwSetWindowUserPointer(window, event);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		Events* events = (Events*)glfwGetWindowUserPointer(window);

		WindowResizeEvent* e = new WindowResizeEvent();
		e->SetHeight(height);
		e->SetWidth(width);
		events->AddEvent(e);
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {

		Events* events = (Events*)glfwGetWindowUserPointer(window);
		WindowCloseEvent* e = new WindowCloseEvent();
		events->AddEvent(e);
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		Events* events = (Events*)glfwGetWindowUserPointer(window);

		switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent* event = new KeyPressedEvent(key, 0);
				events->AddEvent(event);
				break;
			} case GLFW_RELEASE: {
				KeyReleasedEvent* event = new KeyReleasedEvent(key);
				events->AddEvent(event);
				break;
			} case GLFW_REPEAT: {
				KeyPressedEvent* event = new KeyPressedEvent(key, 1);
				events->AddEvent(event);
				break;
			}
		}
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode) {
		Events* events = (Events*)glfwGetWindowUserPointer(window);
		KeyTypedEvent* e = new KeyTypedEvent(keycode);
		events->AddEvent(e);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		Events* events = (Events*)glfwGetWindowUserPointer(window);

		switch (action) {
		case GLFW_PRESS: {
			MausePressedEvent* event = new MausePressedEvent(button);
			events->AddEvent(event);
			break;
		} case GLFW_RELEASE: {
			MauseReleaseEvent* event = new MauseReleaseEvent(button);
			events->AddEvent(event);
			break;
		}
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
		Events* events = (Events*)glfwGetWindowUserPointer(window);

		MauseScrollEvent* event = new MauseScrollEvent(x, y);
		events->AddEvent(event);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
		Events* events = (Events*)glfwGetWindowUserPointer(window);

		MauseCursorEvent* event = new MauseCursorEvent(x, y);
		events->AddEvent(event);
	});
}
void *		Dimension::WindowsWindow::Context			() {
	return window;
}

Dimension::WindowsWindow::~WindowsWindow() {
	glfwTerminate();
}