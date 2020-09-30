#include "GraphicObject.h"

GraphicObject::GraphicObject() {
}

Material * GraphicObject::GetMaterial() {
	return &material;
}

Mesh * GraphicObject::GetMesh() {
	return &mesh;
}

GraphicObject::~GraphicObject() {
}