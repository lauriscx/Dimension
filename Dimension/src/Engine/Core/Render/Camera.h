#pragma once
#include "glm/glm.hpp"

class Camera {
public:
	static void SetPerspective(bool perspective);
	static bool IsPerspective();
	static void SetWindowSize(float width, float height);

	static glm::mat4 GetCameraViewMatrix();
	static void SetProjectionMatrix();

	static glm::mat4 camera;
	static glm::mat4 view;
	static bool perspective;
	static float width;
	static float height;
	static float zNear;
	static float zFar;
	static float fov;

private:
	Camera();
	~Camera();

};