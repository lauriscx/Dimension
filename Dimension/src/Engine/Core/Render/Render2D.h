#include "RenderData/GraphicObject.h"
#include "Shader.h"
#include "glm/glm.hpp"

class Render2D {
public:
	Render2D();

	void SetWindowSize(glm::vec2 WindowSize);
	void SetClearColor(glm::vec4 ClearColor);

	void PrepareScene();
	void StartScene();
	void draw(GraphicObject* object, Dimension::Shader shader);

	~Render2D();

private:
	glm::vec4 ClearColor;
	glm::vec2 WindowSize;
};