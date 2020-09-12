#include "Render2D.h"
#include "GLAD/glad.h"


Render2D::Render2D()
{
}

void Render2D::draw(int index, int size) {
	glDrawElementsBaseVertex(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0, 0);
}

Render2D::~Render2D()
{
}