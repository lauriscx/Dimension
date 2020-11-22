#include "GraphicObject.h"

GraphicObject::GraphicObject() {
}

Material * GraphicObject::GetMaterial() {
	return &material;
}

Batch * GraphicObject::GetBatch() {
	return &mesh;
}

glm::mat4 GraphicObject::GetTransformation() {
	return transformation;
}

glm::mat4 * GraphicObject::SetTransformation(glm::mat4 & transformation) {
	this->transformation = transformation;
	return &this->transformation;
}

GraphicObject::~GraphicObject() {
}