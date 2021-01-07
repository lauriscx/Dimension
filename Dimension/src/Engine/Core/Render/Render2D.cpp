#include "Render2D.h"
#include "GLAD/glad.h"
#include "RenderData/Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Shader.h"

Render2D::Render2D() {
	ClearColor = { 1, 0.75f, 0, 1 };
	BatchElementsSize = 1000000;

	vao.Bind();
	CreateVBO(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_UNSIGNED_INT, 0, BatchElementsSize, 1);//Indicies

	CreateVBO(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT, 0, BatchElementsSize, 3);//Positions
	CreateVBO(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT, 1, BatchElementsSize, 3);//Texturecoords
	CreateVBO(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_INT, 2, BatchElementsSize, 1);//Object index
	CreateVBO(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT, 3, BatchElementsSize, 4);//Colors
	CreateVBO(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_INT, 4, BatchElementsSize, 1);//texture index
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

int Render2D::GetReservedMemoryInMB() {
	int64_t Bytes = sizeof(float) * BatchSizeInBytes;
	int32_t KB = Bytes / 1000;
	int16_t MB = KB / 1000;
	return MB;
}

bool Render2D::IsObjectFit(GraphicObject * GrahicObjectData) {
	return (batch.Indices.size() + GrahicObjectData->Indices.size()) < BatchElementsSize;
}

bool Render2D::IsBatchFull() {
	return batch.Indices.size() >= BatchElementsSize;
}

void Render2D::ResetUniforms() {
	batch.ResetBatchUniforms();
}

void Render2D::UpdateUniforms(GraphicObject* GrahicObjectData) {
	batch.UpdateBatchUniforms(*GrahicObjectData);
}

Render2D::~Render2D() {
}

void Render2D::CreateVBO(int BufferType, int BufferStorageType, int DataType, int location, int ElementsCount, int VariablesCount) {
	VBO* vbo = new VBO();

	vbo->setBufferType(BufferType);
	vbo->setStorageType(BufferStorageType);
	vbo->setDataType(DataType);
	if (BufferType != GL_ELEMENT_ARRAY_BUFFER) {
		vbo->setLocation(location);
	}
	vbo->setSize(VariablesCount);

	vbo->bind();
	vbo->ReserveData(ElementsCount * VariablesCount);
	BatchSizeInBytes += ElementsCount * VariablesCount;
	if (BufferType != GL_ELEMENT_ARRAY_BUFFER) {
		if (DataType == GL_INT) {
			vbo->AttributeISetup();
		}
		else {
			vbo->AttributeSetup();
		}
	}

	vbos.push_back(vbo);
}
