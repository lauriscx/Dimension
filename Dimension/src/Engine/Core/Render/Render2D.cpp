#include "Render2D.h"
#include "GLAD/glad.h"
#include "RenderData/Texture.h"


Render2D::Render2D()
{
}

void Render2D::draw(int index, int size) {
	glDrawElementsBaseVertex(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0, 0);
}

void Render2D::draw(GraphicObject object, Dimension::Shader shader) {
	shader.start();
	int count = object.GetMesh()->getVertexCount();
	shader.sendUniform("Ocolor", object.GetMaterial()->GetColor());
	//unsigned int indices[] = { 0, 1, 2 };
	unsigned int indices[] = { 2, 1, 0, 0, 2, 3 };

	object.GetMesh()->GetVAO().Bind();
	object.GetMaterial()->GetTexture("diffuseMap").ActivateSlot(0);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);
}

Render2D::~Render2D()
{
}