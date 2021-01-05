#include "Render2D.h"
#include "GLAD/glad.h"
#include "RenderData/Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Shader.h"
//#include "../../../Utils/Timer.h"

Render2D::Render2D() {
	ClearColor = { 1, 0.75f, 0, 1 };

	int BatchSize = sizeof(float) * 10000;

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
	vbo_position->setStorageType(GL_DYNAMIC_DRAW);
	vbo_position->setDataType(GL_FLOAT);
	vbo_position->setLocation(0);
	vbo_position->setSize(3);

	vbo_position->bind();
	vbo_position->ReserveData(BatchSize * 3);
	vbo_position->AttributeSetup();

	vbos.push_back(vbo_position);


	VBO* vbo_TextureCoords = new VBO();

	vbo_TextureCoords->setBufferType(GL_ARRAY_BUFFER);
	vbo_TextureCoords->setStorageType(GL_DYNAMIC_DRAW);
	vbo_TextureCoords->setDataType(GL_FLOAT);
	vbo_TextureCoords->setLocation(1);
	vbo_TextureCoords->setSize(3);

	vbo_TextureCoords->bind();
	vbo_TextureCoords->ReserveData(BatchSize * 3);
	vbo_TextureCoords->AttributeSetup();

	vbos.push_back(vbo_TextureCoords);

	VBO* vbo_Indexes = new VBO();

	vbo_Indexes->setBufferType(GL_ARRAY_BUFFER);
	vbo_Indexes->setStorageType(GL_DYNAMIC_DRAW);
	vbo_Indexes->setDataType(GL_INT);
	vbo_Indexes->setLocation(2);
	vbo_Indexes->setSize(1);

	vbo_Indexes->bind();
	vbo_Indexes->ReserveData(BatchSize);
	vbo_Indexes->AttributeISetup();

	vbos.push_back(vbo_Indexes);

	VBO* vbo_Colors = new VBO();

	vbo_Colors->setBufferType(GL_ARRAY_BUFFER);
	vbo_Colors->setStorageType(GL_DYNAMIC_DRAW);
	vbo_Colors->setDataType(GL_FLOAT);
	vbo_Colors->setLocation(3);
	vbo_Colors->setSize(4);

	vbo_Colors->bind();
	vbo_Colors->ReserveData(BatchSize * 4);
	vbo_Colors->AttributeSetup();

	vbos.push_back(vbo_Colors);

	VBO* vbo_TexturesIndexes = new VBO();

	vbo_TexturesIndexes->setBufferType(GL_ARRAY_BUFFER);
	vbo_TexturesIndexes->setStorageType(GL_DYNAMIC_DRAW);
	vbo_TexturesIndexes->setDataType(GL_INT);
	vbo_TexturesIndexes->setLocation(4);
	vbo_TexturesIndexes->setSize(1);

	vbo_TexturesIndexes->bind();
	vbo_TexturesIndexes->ReserveData(BatchSize);
	vbo_TexturesIndexes->AttributeISetup();

	vbos.push_back(vbo_TexturesIndexes);
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
	drawCalls = 0;
}

void Render2D::StartBatching() {
	batchTime.Start();
}

void Render2D::Stopbatching() {
	batchTime.Stop();
}


void Render2D::PackObject(GraphicObject* object, Dimension::Shader shader) {
	batch.AddToBatch(*object);
	textures.push_back(object->GetMaterial()->GetTexture("diffuseMap"));
	RenderObjectCount++;
}

void Render2D::ClearBatch() {
	batch.ClearBatch();
	textures.clear();
	RenderObjectCount = 0;
}

void Render2D::flush(Dimension::Shader shader) {
	if (batch.GetObjectCount() < 1) {
		return;
	}
	renderTime.Start();
	streamDataTime.Start();
	/*vbos[0]->bind();
	vbos[0]->StoreData(&batch.Indices[0], batch.Indices.size(), 0);
	vbos[0]->unbind();*/
	
	vbos[1]->bind();
	vbos[1]->StoreData(&batch.Positions[0], batch.Positions.size(), 0);
	vbos[1]->unbind();

	vbos[2]->bind();
	vbos[2]->StoreData(&batch.TexturesCoordinates[0], batch.TexturesCoordinates.size(), 0);
	vbos[2]->unbind();

	vbos[3]->bind();
	vbos[3]->StoreData(&batch.VertextObjectIndex[0], batch.VertextObjectIndex.size(), 0);
	vbos[3]->unbind();


	vbos[4]->bind();
	vbos[4]->StoreData(&batch.Colors[0], batch.Colors.size(), 0);
	vbos[4]->unbind();


	vbos[5]->bind();
	vbos[5]->StoreData(&batch.TexturesIndex[0], batch.TexturesIndex.size(), 0);
	vbos[5]->unbind();

	streamDataTime.Stop();
	drawTime.Start();

	Camera::SetProjectionMatrix();
	shader.start();
	shader.sendUniform("ModeltransformationArray", &batch.transformations[0], batch.GetObjectCount());
	int a = 0;
	for (glm::vec4 color : batch.ObjectColor) {
		shader.sendUniform("colors[" + std::to_string(a) + "]", batch.ObjectColor[a]);
		a++;
	}
	shader.sendUniform("ProjectionView", Camera::GetCameraViewMatrix());

	vao.Bind();
	int i = 0;
	for (Texture texture : textures) {
		texture.ActivateSlot(shader.getTextureSlot("textures[" + std::to_string(i) + "]"));
		i++;
	}

	glDrawElements(GL_TRIANGLES, batch.Indices.size(), GL_UNSIGNED_INT, &batch.Indices[0]);
	drawCalls++;

	vao.Unbind();

	drawTime.Stop();
	renderTime.Stop();
}

float Render2D::GetRenderTime() {
	return renderTime.DeltaTime();
}

int Render2D::GetObjectsCount() {
	return RenderObjectCount;
}

int Render2D::DrawCalls() {
	return drawCalls;
}

float Render2D::GetDrawTime() {
	return drawTime.DeltaTime();
}

float Render2D::GetBatchTime() {
	return batchTime.DeltaTime();
}

float Render2D::GetStreamData() {
	return streamDataTime.DeltaTime();
}

void Render2D::ResetUniforms() {
	batch.ResetBatchUniforms();
}

void Render2D::UpdateUniforms(GraphicObject* GrahicObjectData) {
	batch.UpdateBatchUniforms(*GrahicObjectData);
}




Render2D::~Render2D() {
}