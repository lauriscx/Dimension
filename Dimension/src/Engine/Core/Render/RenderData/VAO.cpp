#include "VAO.h"
#include <glad/glad.h>
#include <iostream>

VAO::VAO() {
	//Generate vertex array object.
	glGenVertexArrays(1, &VAOID);
}

//Activate object.
void VAO::Bind() {
	glBindVertexArray(VAOID);
}
//Disable object.
void VAO::Unbind() {
	glBindVertexArray(0);
}

//Clean up data.
void VAO::CleanUp() {
	Unbind();
	glDeleteVertexArrays(1, &VAOID);
}
VAO::~VAO()
{
}