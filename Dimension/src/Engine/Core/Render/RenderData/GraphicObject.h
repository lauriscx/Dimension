#pragma once
#include "Material.h"
#include "glm/glm.hpp"
//#include "Batch.h"
#include <vector>

class GraphicObject {
public:
	GraphicObject();

	Material* GetMaterial();
	/*Batch* GetBatch();
	void SetBatch(Batch* batch);*/
	glm::mat4 GetTransformation();

	glm::mat4* SetTransformation(glm::mat4& transformation);

	~GraphicObject();

	std::vector<unsigned int> Indices;
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

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
private:
	Material material;

	glm::mat4 transformation;
};