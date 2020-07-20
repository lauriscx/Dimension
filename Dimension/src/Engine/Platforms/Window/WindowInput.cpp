#pragma once
#include "../../Core/Input.h"
#include "../../Core/Aplication.h"
#include "WindowsWindow.h"
#include <GLFW/glfw3.h>

bool						Dimension::Input::IsKeyPressed		(int Key) {
	auto window = static_cast<GLFWwindow*>(Dimension::Aplication::Get()->GetWindow()->Context());
	auto state = glfwGetKey(window, Key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool						Dimension::Input::IsMousePressed	(int Key) {
	auto window = static_cast<GLFWwindow*>(Dimension::Aplication::Get()->GetWindow()->Context());
	auto state = glfwGetMouseButton(window, Key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
std::pair<double, double>	Dimension::Input::GetMousePosition	() {
	auto window = static_cast<GLFWwindow*>(Dimension::Aplication::Get()->GetWindow()->Context());
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return { x, y };
}
double						Dimension::Input::GetMouseX			() {
	auto[x, y] = GetMousePosition();
	return x;
}
double						Dimension::Input::GetMouseY			() {
	auto[x, y] = GetMousePosition();
	return y;
}