#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "GraphicObject.h"

class Batch {
public:
	void AddBatch(Batch batch);
	void AddToBatch(GraphicObject GrahicObjectData);
	void ResetBatchUniforms();
	void UpdateBatchUniforms(GraphicObject GrahicObjectData);

	void ClearBatch();

	std::vector<int> Indices;
	std::vector<int> VertextObjectIndex;
	std::vector<int> UniformsIndex;
	std::vector<int> TexturesIndex;

	std::vector<float> Positions;
	std::vector<float> Normals;
	std::vector<float> Tangents;
	std::vector<float> BiTangents;

	std::vector<float> Colors;

	std::vector<float> TexturesCoordinates;

	//For animations 3D
	std::vector<int> BonesIndex;
	std::vector<float> BonesWeights;

	std::vector<glm::mat4> transformations;
	std::vector<glm::vec4> ObjectColor;

	inline int GetObjectCount() { return ObjectCount; }

	std::vector<Texture> TexturesBatch;

	bool NeedRefresh = true;

private:
	int IndexNumber = 0;
	int ObjectCount = 0;
};