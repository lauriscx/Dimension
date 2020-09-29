#pragma once
#include "Mesh.h"
#include "Material.h"

class GraphicObject {
public:
	GraphicObject();

	Material* GetMaterial();
	Mesh* GetMesh();

	~GraphicObject();

private:
	Material material;
	Mesh mesh;
};