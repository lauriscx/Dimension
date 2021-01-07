#pragma once
#include "RenderData/GraphicObject.h"
#include "glm/glm.hpp"
#include "RenderData/Batch.h"
#include "RenderData/VAO.h"
#include "RenderData/VBO.h"
#include <vector>
#include "../../../Utils/Timer.h"

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
	void StartBatching();
	void Stopbatching();
	void PackObject(GraphicObject* object, Dimension::Shader shader);
	void ClearBatch();
	void flush(Dimension::Shader shader);
	float GetRenderTime();
	int GetObjectsCount();
	int DrawCalls();
	float GetDrawTime();
	float GetBatchTime();
	float GetStreamData();
	int GetReservedMemoryInMB();
	bool IsObjectFit(GraphicObject* GrahicObjectData);
	bool IsBatchFull();

	void Updatebatch();
	void ResetUniforms();
	void UpdateUniforms(GraphicObject* GrahicObjectData);

	~Render2D();

private:
	glm::vec4 ClearColor;
	glm::vec2 WindowSize;

	Batch batch;
	std::vector<Texture> textures;
	Timer renderTime;
	Timer batchTime;
	Timer drawTime;
	Timer streamDataTime;
	int RenderObjectCount = 0;
	int drawCalls = 0;
	int BatchElementsSize = 0;
	int64_t BatchSizeInBytes;
	/*Renderig data for GPU*/
	//Vertex array object store all vertex buffer objects (vbo)
	VAO vao;
	//Save data in vbo about 3D or 2D object.
	std::vector<VBO*> vbos;

	void CreateVBO(int BufferType, int BufferStorageType, int DataType, int location, int ElementsCount, int VariablesCount);
};