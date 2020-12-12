#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

glm::mat4 Camera::camera = glm::mat4(1.0f);
glm::mat4 Camera::view = glm::mat4(1.0f);
bool Camera::perspective = false;

float Camera::width = 1;
float Camera::height = 1;
float Camera::zNear = -0.1f;
float Camera::zFar = 1000.0f;
float Camera::fov = 75.0f;

glm::vec3	Camera::Position = glm::vec3(0, 0.5f, 0);
glm::vec3	Camera::ViewDirection = glm::vec3(0, 0, -1);
glm::vec2	Camera::Rotation = glm::vec2(0, 0);

/* Axis rotate on left and right */
glm::vec3	Camera::UpAxis = glm::vec3(0, 1, 0);

/* Proportion rotate up and down */
glm::vec3	Camera::ProportionRotation = glm::vec3(0, 0, 0);
glm::mat4	Camera::ProportionRotationMatrixUpdated = glm::mat4(1);

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
	return camera * GetLookAt();
}

void Camera::SetProjectionMatrix() {
	if (perspective) {
		camera = glm::perspective(/*glm::radians(*/fov/*)*/, width / height, zNear, zFar);
	}
	else {
		camera = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, zNear, zFar);
	}
}


void Camera::SetFieldOfView(float		FieldOfView) {
	Camera::fov = FieldOfView;
}
void Camera::SetViewDirection(glm::vec3	ViewDirection) {
	Camera::ViewDirection = ViewDirection;
}
void Camera::SetViewDistance(glm::vec2	Distance) {
	Camera::zNear = Distance.x;
	Camera::zFar = Distance.y;
}
void Camera::SetUpAxis(glm::vec3	UpAxis) {
	Camera::UpAxis = UpAxis;
}
void Camera::SetPosition(glm::vec3	Position) {
	Camera::Position = Position;
}
void Camera::SetRotation(glm::vec2	Rotation) {
	Camera::Rotation = Rotation;
}
void Camera::Rotate(glm::vec2	Rotation) {
	Camera::Rotation = Rotation;
}

void Camera::CameraMove(glm::vec3 Position, float Speed) {
	if (Position.x > 0.0f) {
		ProportionRotation = glm::cross(ViewDirection, UpAxis);
		Camera::Position += ProportionRotation * glm::vec3(1.0f, 0.0f, 1.0f) * Speed;
	}
	if (Position.x < 0.0f) {
		ProportionRotation = glm::cross(ViewDirection, UpAxis);
		Camera::Position -= ProportionRotation * Speed;

	}
	if (Position.z > 0.0f) {
		Camera::Position += ViewDirection * glm::vec3(1.0f, 0.0f, 1.0f) * Speed;
	}
	if (Position.z < 0.0f) {
		Camera::Position -= ViewDirection * glm::vec3(1.0f, 0.0f, 1.0f) * Speed;
	}
	if (Position.y > 0.0f) {
		Camera::Position += UpAxis * Speed;
	}
	if (Position.y < 0.0f) {
		Camera::Position -= UpAxis * Speed;
	}
}

glm::mat4 Camera::GetLookAt() {
	ProportionRotation = glm::cross(ViewDirection, UpAxis);

	ProportionRotationMatrixUpdated = glm::rotate(glm::mat4(1.0f), -Rotation.x, UpAxis) * glm::rotate(glm::mat4(1.0f), -Rotation.y, ProportionRotation);

	/* Rotation view direction vector */
	ViewDirection = glm::mat3(ProportionRotationMatrixUpdated) * ViewDirection;

	return glm::lookAt(Position, Position + ViewDirection, UpAxis);
}
glm::mat4 Camera::GetPerspective() {
	return glm::perspective(fov, width / height, zNear, zFar);
}
glm::mat4 Camera::GetOrtho() {
	return  glm::ortho(0.0f, width, 0.0f, height, zNear, zFar);
}
glm::vec3 Camera::GetPosition() {
	return Position;
}
glm::vec3 Camera::GetViewDirection() {
	return ViewDirection;
}
glm::vec3 Camera::GetUpAxis() {
	return UpAxis;
}
glm::vec2 Camera::GetRotation() {
	return Rotation;
}
float Camera::GetFieldOfView() {
	return fov;
}

Camera::~Camera(){}