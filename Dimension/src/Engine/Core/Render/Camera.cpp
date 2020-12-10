#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

glm::mat4 Camera::camera = glm::mat4(1.0f);
glm::mat4 Camera::view = glm::mat4(1.0f);
bool Camera::perspective = false;

float Camera::width = 800;
float Camera::height = 600;
float Camera::zNear = 0.1f;
float Camera::zFar = 10.0f;
float Camera::fov = 75.0f;

Camera::Camera() {}

void Camera::SetPerspective(bool perspective) {
	Camera::perspective = perspective;
}

bool Camera::IsPerspective() {
	return perspective;
}

void Camera::SetWindowSize(float width, float height) {
	Camera::width = width;
	Camera::height = height;
}


glm::mat4 Camera::GetCameraViewMatrix() {
	return camera * view;
}

void Camera::SetProjectionMatrix() {
	view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (perspective) {
		camera = glm::perspective(glm::radians(fov), width / height, zNear, zFar);
	}
	else {
		camera = glm::ortho(0.0f, width, 0.0f, height, zNear, zFar); 
	}
}

Camera::~Camera(){}