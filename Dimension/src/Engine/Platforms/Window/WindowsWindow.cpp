#include "WindowsWindow.h"
#include "GLFW/glfw3.h"
#include "../../Core/Input/Events/Events.h"
#include "../../Core/Input/Events/AplicationEvent.h"
#include "../../Core/Input/Events/MouseEvent.h"
#include "../../Core/Input/Events/KeyEvent.h"
#include <iostream>


Dimension::WindowsWindow::WindowsWindow	(const char * title, const uint32_t width, const uint32_t height) {
	/* Initialize the library */
	if (!glfwInit())
		return;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, title, NULL, NULL);

	if (!window) {
		glfwTerminate();
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	this->height = height;
	this->width = width;
	this->title = title;
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
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT);

	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
}
void		Dimension::WindowsWindow::EventsHandler		(Events* event) {
	//Pass events class to glfw for events input.
	glfwSetWindowUserPointer(window, event);
	
	// Set GLFW callbacks
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		Events& events = *(Events*)glfwGetWindowUserPointer(window);

		WindowResizeEvent e;
		e.SetHeight(height);
		e.SetWidth(width);
		events.AddEvent(e);
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {

		Events& events = *(Events*)glfwGetWindowUserPointer(window);
		WindowCloseEvent e;
		events.AddEvent(e);
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		Events& events = *(Events*)glfwGetWindowUserPointer(window);

		switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent event(key, 0);
				events.AddEvent(event);
				break;
			} case GLFW_RELEASE: {
				KeyReleasedEvent event(key);
				events.AddEvent(event);
				break;
			} case GLFW_REPEAT: {
				KeyPressedEvent event(key, 1);
				events.AddEvent(event);
				break;
			}
		}
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode) {
		Events& events = *(Events*)glfwGetWindowUserPointer(window);
		KeyTypedEvent e(keycode);
		events.AddEvent(e);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		Events& events = *(Events*)glfwGetWindowUserPointer(window);

		switch (action) {
		case GLFW_PRESS: {
			MausePressedEvent event(button);
			events.AddEvent(event);
			break;
		} case GLFW_RELEASE: {
			MauseReleaseEvent event(button);
			events.AddEvent(event);
			break;
		}
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
		Events& events = *(Events*)glfwGetWindowUserPointer(window);

		MauseScrollEvent event(x, y);
		events.AddEvent(event);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
		Events& events = *(Events*)glfwGetWindowUserPointer(window);

		MauseCursorEvent event(x, y);
		events.AddEvent(event);
	});
}
void *		Dimension::WindowsWindow::Context			() {
	return window;
}

Dimension::WindowsWindow::~WindowsWindow() {
	glfwTerminate();
}