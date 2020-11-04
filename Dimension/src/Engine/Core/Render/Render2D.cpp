#include "Render2D.h"
#include "GLAD/glad.h"
#include "RenderData/Texture.h"
#include "glm/gtc/matrix_transform.hpp"

Render2D::Render2D() {
	ClearColor = { 1, 0.75f, 0, 1 };
}

void Render2D::SetWindowSize(glm::vec2 WindowSize) {
	this->WindowSize = WindowSize;
}

void Render2D::SetClearColor(glm::vec4 ClearColor) {
	this->ClearColor = ClearColor;
}

void Render2D::PrepareScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);

	glViewport(0, 0, WindowSize.x, WindowSize.y);
}

void Render2D::StartScene() {

}

void Render2D::draw(GraphicObject* object, Dimension::Shader shader) {
	shader.start();
	shader.sendUniform("ModelTransformation", *object->GetTransformation());
	shader.sendUniform("Ocolor", object->GetMaterial()->GetColor());

	object->GetMesh()->GetVAO().Bind();
	object->GetMaterial()->GetTexture("diffuseMap").ActivateSlot(0);

	glDrawElements(GL_TRIANGLES, object->GetMesh()->getVertexCount(), GL_UNSIGNED_INT, &object->GetMesh()->GetIndices()[0]);
}



Render2D::~Render2D() {
}