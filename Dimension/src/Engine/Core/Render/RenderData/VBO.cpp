#include "VBO.h"
#include <glad/glad.h>
#include <iostream>

VBO::VBO() {
	glGenBuffers(1, &id);
	//std::cout << "VBO constructed" << std::endl;
	bufferType = GL_ARRAY_BUFFER;
	storageType = GL_STATIC_DRAW;
	dataType = GL_FLOAT;
	location = 0;
	stride = 0;
	offset = 0;
	normalized = false;
}


//Activate buffer.
void VBO::bind() {
	/*Enables vertex buffer object*/
	glBindBuffer(GL_ARRAY_BUFFER, id);

	/*Enables vertex array buffer attribute location if it is not element array*/
	if (bufferType != GL_ELEMENT_ARRAY_BUFFER) {
		enableAttributeArray();
	}
}

void VBO::setStorageType(int storageType) {
	this->storageType = storageType;
}
void VBO::setNormalized	(bool normalized) {
	this->normalized = normalized;
}
void VBO::setBufferType	(int bufferType) {
	this->bufferType = bufferType;
}
void VBO::setDataType	(int dataType) {
	this->dataType = dataType;
}
void VBO::setLocation	(int location) {
	this->location = location;
}
void VBO::setStride		(int stride) {
	this->stride = stride;
}
void VBO::setOffset		(int offset) {
	this->offset = offset;
}
void VBO::setSize		(int size) {
	this->size = size;
}

//Disable buffer.
void VBO::unbind() {
	/*Disables vertex buffer object*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*Disables vertex array buffer attribute location if it is not element array*/
	if (bufferType != GL_ELEMENT_ARRAY_BUFFER) {
		disableAttributeArray();
	}
}

//Enable attribute array by index witch is set in shader class.
void VBO::enableAttributeArray() {
	glEnableVertexAttribArray(location);
}
//Disable attribute array by index witch is set in shader class.
void VBO::disableAttributeArray() {
	glDisableVertexAttribArray(location);
}


void VBO::AttribDivisor(int Incrament) {
	/*	glVertexAttribDivisor modifies the rate at which generic vertex attributes
	 * 	advance when rendering multiple instances of primitives in a single draw call.
	 * 	If divisor is zero, the attribute at slot index advances once per vertex.
	 * 	If divisor is non-zero, the attribute advances once per divisor instances of
	 * 	the set(s) of vertices being rendered.
	 * 	An attribute is referred to as instanced if its GL_VERTEX_ATTRIB_ARRAY_DIVISOR value is non-zero.
	 * 	index must be less than the value of GL_MAX_VERTEX_ATTRIBS.
	 * */
	glVertexAttribDivisor(size, Incrament);
}
void VBO::AttributeSetup() {
	/*
	 * Say to opengl witch buffer(location) in shader program,
	 * how many variables(size) to use for 1 vertex,
	 * dataType(for variable type for example float, int, double),
	 * normalize is values is rounded,
	 * stride how many variables to skip,
	 * offset from witch variable to start.
	 * */
	/* glVertexAttribPointer(
	 0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	 3,                  // size
	 GL_FLOAT,           // type
	 GL_FALSE,           // normalized?
	 0,                  // stride
	 (void*)0            // array buffer offset
		);*/
	glVertexAttribPointer(location, size, dataType, normalized, stride, (void*)offset);
}
void VBO::AttributeISetup() {
	/*
	 * Say to opengl witch buffer(location) in shader program,
	 * how many variables(size) to use for 1 vertex,
	 * dataType(for variable type for example float, int, double),
	 * stride how many variables to skip,
	 * offset from witch variable to start.
	 * */
	glVertexAttribIPointer(location, size, dataType, stride, (void*)offset);
}

/*Reserve data in buffer for latter to add*/
void VBO::ReserveData(int Size) {
	glBufferData(bufferType, sizeof(int) * Size, (void*)0, storageType);
}

/*Store data functions*/
void VBO::StoreData(long* data) {
	glBufferData(bufferType, 3, data, storageType);
	//glBufferData(bufferType, sizeof(long) * Size, (void*)Size, storageType);
}
void VBO::StoreData(int* data, int _size) {
	glBufferData(bufferType, _size, data, storageType);
}
void VBO::StoreData(unsigned int* data, int _size) {
	glBufferData(bufferType, _size, data, storageType);
}
void VBO::StoreData(float* data, int _size) {
	glBufferData(bufferType, _size, data, storageType);
}
void VBO::StoreData(double* data) {
	glBufferData(bufferType, 3, data, storageType);
	//glBufferData(bufferType, sizeof(double) * Size, (void*)Size, storageType);
}

void VBO::StoreData(long* Data, int size, int Offset) {
	glBufferSubData(bufferType, Offset, sizeof(long) * size, Data);
}
void VBO::StoreData(int* Data, int size, int Offset) {
	glBufferSubData(bufferType, Offset, sizeof(int) * size, Data);
}
void VBO::StoreData(float* Data, int size, int Offset) {
	glBufferSubData(bufferType, Offset, sizeof(float) * size, Data);
}
void VBO::StoreData(double* Data, int size, int Offset) {
	glBufferSubData(bufferType, Offset, sizeof(double) * size, Data);
}

//Clean up data
void VBO::CleanUp() {
	unbind();
	glDeleteBuffers(1, &id);
	//std::cout << "VBO deleted" << std::endl;
}


VBO::~VBO()
{
	//std::cout << "VBO deconstructed" << std::endl;
}