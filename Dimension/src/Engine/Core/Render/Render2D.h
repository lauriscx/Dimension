#include "RenderData/GraphicObject.h"
#include "glm/glm.hpp"
#include "RenderData/Batch.h"
#include "RenderData/VAO.h"
#include "RenderData/VBO.h"
#include <vector>

class Texture;
namespace Dimension {
	class Shader;
};

class Render2D {
public:
	Render2D();

	void SetWindowSize(glm::vec2 WindowSize);
	void SetClearColor(glm::vec4 ClearColor);

	void PrepareScene();
	void StartScene();
	void PackObject(GraphicObject object);
	void flush(Dimension::Shader shader);

	~Render2D();

private:
	glm::vec4 ClearColor;
	glm::vec2 WindowSize;

	Batch batch;
	std::vector<Texture> textures;

	/*Renderig data for GPU*/
	//Vertex array object store all vertex buffer objects (vbo)
	VAO vao;
	//Save data in vbo about 3D or 2D object.
	std::vector<VBO*> vbos;

	std::vector<int> indices;
};