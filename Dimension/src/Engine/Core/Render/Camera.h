#pragma once
#include "glm/glm.hpp"

class Camera {
public:
	static void SetPerspective(bool perspective);
	static bool IsPerspective();
	static void SetWindowSize(float width, float height);

	static glm::mat4 GetCameraViewMatrix();
	static void SetProjectionMatrix();

	static glm::mat4 GetLookAt();
	static glm::mat4 GetPerspective();
	static glm::mat4 GetOrtho();
	static glm::vec3 GetPosition();
	static glm::vec3 GetViewDirection();
	static glm::vec3 GetUpAxis();
	static glm::vec2 GetRotation();
	static float GetFieldOfView();

	static void SetFieldOfView(float		FieldOfView);
	static void SetViewDirection(glm::vec3	Direction);
	static void SetViewDistance(glm::vec2	Distance);
	static void SetUpAxis(glm::vec3	UpAxis);
	static void SetPosition(glm::vec3	Position);
	static void SetRotation(glm::vec2	Rotation);
	static void Rotate(glm::vec2	Rotation);

	static void CameraMove(glm::vec3	Position, float Speed);

	static glm::mat4 camera;
	static glm::mat4 view;
	static bool perspective;
	static float width;
	static float height;
	static float zNear;
	static float zFar;
	static float fov;

	static glm::vec3	Position;
	static glm::vec3	ViewDirection;
	static glm::vec2	Rotation;

	/* Axis rotate on left and right */
	static glm::vec3	UpAxis;

	/* Proportion rotate up and down */
	static glm::vec3	ProportionRotation;
	static glm::mat4	ProportionRotationMatrixUpdated;


private:
	Camera();
	~Camera();
};