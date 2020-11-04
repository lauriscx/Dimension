#pragma once
#include "Mesh.h"
#include "Material.h"
#include "glm/glm.hpp"

class GraphicObject {
public:
	GraphicObject();

	Material* GetMaterial();
	Mesh* GetMesh();
	glm::mat4 GetTransformation();

	glm::mat4* SetTransformation(glm::mat4& transformation);

	~GraphicObject();

private:
	Material material;
	Mesh mesh;
	glm::mat4 transformation;
};