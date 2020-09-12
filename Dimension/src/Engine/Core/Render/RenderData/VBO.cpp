#include "VBO.h"
#include <glad/glad.h>

VBO::VBO()
{
}


//Activate buffer.
void VBO::bind() {
	/*Enables vertex buffer object*/
	glBindBuffer(bufferType, id);

	/*Enables vertex array buffer attribute location if it is not element array*/
	if (bufferType != GL_ELEMENT_ARRAY_BUFFER) {
		enableAttributeArray();
	}
}

//Disable buffer.
void VBO::unbind() {
	/*Disables vertex buffer object*/
	glBindBuffer(bufferType, 0);

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
	glBufferData(bufferType, sizeof(int) * Size, (void*)Size, storageType);
}

/*Store data functions*/
void VBO::StoreData(long* data) {
	//glBufferData(bufferType, sizeof(long) * Size, (void*)Size, storageType);
}
void VBO::StoreData(int* data) {
	//glBufferData(bufferType, sizeof(int) * Size, (void*)Size, storageType);
}
void VBO::StoreData(float* data) {
	//glBufferData(bufferType, sizeof(float) * Size, (void*)Size, storageType);
}
void VBO::StoreData(double* data) {
	//glBufferData(bufferType, sizeof(double) * Size, (void*)Size, storageType);
}

void VBO::StoreData(long* Data, int Offset) {
	//glBufferSubData(bufferType, Offset, Data);
}
void VBO::StoreData(int* Data, int Offset) {
	//glBufferSubData(bufferType, Offset, Data);
}
void VBO::StoreData(float* Data, int Offset) {
	//glBufferSubData(bufferType, Offset, Data);
}
void VBO::StoreData(double* Data, int Offset) {
	//glBufferSubData(bufferType, Offset, Data);
}

//Clean up data
void VBO::CleanUp() {
	unbind();
	glDeleteBuffers(1, &id);
}


VBO::~VBO()
{
}