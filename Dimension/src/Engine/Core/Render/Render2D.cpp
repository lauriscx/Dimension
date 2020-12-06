#include "Render2D.h"
#include "GLAD/glad.h"
#include "RenderData/Texture.h"
#include "glm/gtc/matrix_transform.hpp"

Render2D::Render2D() {
	ClearColor = { 1, 0.75f, 0, 1 };

	vao.Bind();

	VBO* ibo_indices = new VBO();
	ibo_indices->setBufferType(GL_ELEMENT_ARRAY_BUFFER);
	ibo_indices->setStorageType(GL_STATIC_DRAW);
	ibo_indices->setDataType(GL_UNSIGNED_INT);

	ibo_indices->bind();
	ibo_indices->ReserveData(50);

	vbos.push_back(ibo_indices);

	VBO* vbo_position = new VBO();

	vbo_position->setBufferType(GL_ARRAY_BUFFER);
	vbo_position->setStorageType(GL_STATIC_DRAW);
	vbo_position->setDataType(GL_FLOAT);
	vbo_position->setLocation(0);
	vbo_position->setSize(3);

	vbo_position->bind();
	vbo_position->ReserveData(50);
	vbo_position->AttributeSetup();

	vbos.push_back(vbo_position);


	VBO* vbo_TextureCoords = new VBO();

	vbo_TextureCoords->setBufferType(GL_ARRAY_BUFFER);
	vbo_TextureCoords->setStorageType(GL_STATIC_DRAW);
	vbo_TextureCoords->setDataType(GL_FLOAT);
	vbo_TextureCoords->setLocation(1);
	vbo_TextureCoords->setSize(3);

	vbo_TextureCoords->bind();
	vbo_TextureCoords->ReserveData(50);
	vbo_TextureCoords->AttributeSetup();

	vbos.push_back(vbo_TextureCoords);
	vao.Unbind();
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

void Render2D::PackObject(GraphicObject object) {
	batch.AddToBatch(object);
}

void Render2D::flush(GraphicObject* object, Dimension::Shader shader) {
	vao.Unbind();

	vbos[0]->bind();
	vbos[0]->StoreData(&batch.Indices[0], batch.Indices.size(), 0);
	vbos[0]->unbind();
	
	vbos[1]->bind();
	vbos[1]->StoreData(&batch.Positions[0], batch.Positions.size(), 0);
	vbos[1]->unbind();

	vbos[2]->bind();
	vbos[2]->StoreData(&batch.TexturesCoordinates[0], batch.TexturesCoordinates.size(), 0);
	vbos[2]->unbind();
	

	shader.start();
	shader.sendUniform("ModelTransformation", object->GetTransformation());
	shader.sendUniform("Ocolor", object->GetMaterial()->GetColor());

	//object->GetMesh()->GetVAO().Bind();
	vao.Bind();
	object->GetMaterial()->GetTexture("diffuseMap").ActivateSlot(0);

	glDrawElements(GL_TRIANGLES, batch.Indices.size(), GL_UNSIGNED_INT, &batch.Indices[0]);

	vao.Unbind();

	batch.ClearBatch();
}



Render2D::~Render2D() {
}