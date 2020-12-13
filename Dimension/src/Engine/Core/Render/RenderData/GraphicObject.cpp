#include "GraphicObject.h"
#include "glm/gtc/matrix_transform.hpp"

GraphicObject::GraphicObject() {
	position = glm::vec3(0, 0, -10.0f);
	rotation = glm::vec3(0);
	scale = glm::vec3(0.1f);
	transformation = glm::mat4(1);
}

Material * GraphicObject::GetMaterial() {
	return &material;
}

/*Batch * GraphicObject::GetBatch() {
	return &mesh;
}

void GraphicObject::SetBatch(Batch * batch)
{
	this->mesh = *batch;
}*/

glm::mat4 GraphicObject::GetTransformation() {
	transformation = glm::translate(transformation, position);
	transformation = glm::rotate(transformation, rotation.x, glm::vec3(1.0f, 0, 0));
	transformation = glm::rotate(transformation, rotation.y, glm::vec3(0, 1.0f, 0));
	transformation = glm::rotate(transformation, rotation.z, glm::vec3(0, 0, 1.0f));
	transformation = glm::scale(transformation, scale);
	return transformation;
}

glm::mat4 * GraphicObject::SetTransformation(glm::mat4 & transformation) {
	this->transformation = transformation;
	return &this->transformation;
}

GraphicObject::~GraphicObject() {
}