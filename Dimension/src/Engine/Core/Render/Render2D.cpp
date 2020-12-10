#include "Render2D.h"
#include "GLAD/glad.h"
#include "RenderData/Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"

Render2D::Render2D() {
	ClearColor = { 1, 0.75f, 0, 1 };

	int BatchSize = 10000;

	vao.Bind();

	VBO* ibo_indices = new VBO();
	ibo_indices->setBufferType(GL_ELEMENT_ARRAY_BUFFER);
	ibo_indices->setStorageType(GL_STATIC_DRAW);
	ibo_indices->setDataType(GL_UNSIGNED_INT);

	ibo_indices->bind();
	ibo_indices->ReserveData(BatchSize);

	vbos.push_back(ibo_indices);

	VBO* vbo_position = new VBO();

	vbo_position->setBufferType(GL_ARRAY_BUFFER);
	vbo_position->setStorageType(GL_STREAM_DRAW);
	vbo_position->setDataType(GL_FLOAT);
	vbo_position->setLocation(0);
	vbo_position->setSize(3);

	vbo_position->bind();
	vbo_position->ReserveData(BatchSize * 3);
	vbo_position->AttributeSetup();

	vbos.push_back(vbo_position);


	VBO* vbo_TextureCoords = new VBO();

	vbo_TextureCoords->setBufferType(GL_ARRAY_BUFFER);
	vbo_TextureCoords->setStorageType(GL_STREAM_DRAW);
	vbo_TextureCoords->setDataType(GL_FLOAT);
	vbo_TextureCoords->setLocation(1);
	vbo_TextureCoords->setSize(3);

	vbo_TextureCoords->bind();
	vbo_TextureCoords->ReserveData(BatchSize * 3);
	vbo_TextureCoords->AttributeSetup();

	vbos.push_back(vbo_TextureCoords);

	VBO* vbo_Indexes = new VBO();

	vbo_Indexes->setBufferType(GL_ARRAY_BUFFER);
	vbo_Indexes->setStorageType(GL_STREAM_DRAW);
	vbo_Indexes->setDataType(GL_INT);
	vbo_Indexes->setLocation(2);
	vbo_Indexes->setSize(1);

	vbo_Indexes->bind();
	vbo_Indexes->ReserveData(BatchSize);
	vbo_Indexes->AttributeISetup();

	vbos.push_back(vbo_Indexes);
	vao.Unbind();
}

void Render2D::SetWindowSize(glm::vec2 WindowSize) {
	this->WindowSize = WindowSize;
}

void Render2D::SetClearColor(glm::vec4 ClearColor) {
	this->ClearColor = ClearColor;
}

void Render2D::PrepareScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	vbos[3]->bind();
	vbos[3]->StoreData(&batch.VertextObjectIndex[0], batch.VertextObjectIndex.size(), 0);
	vbos[3]->unbind();
	
	shader.start();
	shader.sendUniform("ModeltransformationArray", &batch.transformations[0], batch.GetObjectCount());
	shader.sendUniform("Ocolor", object->GetMaterial()->GetColor());
	shader.sendUniform("ProjectionView", Camera::GetCameraViewMatrix());

	vao.Bind();
	object->GetMaterial()->GetTexture("diffuseMap").ActivateSlot(0);

	glDrawElements(GL_TRIANGLES, batch.Indices.size(), GL_UNSIGNED_INT, &batch.Indices[0]);

	vao.Unbind();

	batch.ClearBatch();
}




Render2D::~Render2D() {
}