#pragma once
#include "Material.h"
#include "glm/glm.hpp"
#include "Batch.h"

class GraphicObject {
public:
	GraphicObject();

	Material* GetMaterial();
	Batch* GetBatch();
	glm::mat4 GetTransformation();

	glm::mat4* SetTransformation(glm::mat4& transformation);

	~GraphicObject();

private:
	Material material;
	Batch mesh;

	glm::mat4 transformation;
};