#include "Texture.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../../../../Utils/stb_image.h"

Texture::Texture() {
	textureType				= GL_TEXTURE_2D;
	filter					= GL_LINEAR_MIPMAP_LINEAR;
	wrap					= GL_CLAMP_TO_EDGE;
	mipMapLevel				= 0.0f;
	internalFormat			= GL_RGBA;
	pixelDataFromat			= GL_RGBA;
	antisotropyFilterLevel	= 0.0f;
	compareFunc				= GL_NONE;
	compareMode				= GL_NONE;
	dephtMode				= GL_NONE;
	pixelDataType			= GL_UNSIGNED_BYTE;
	/* Create texture buffer for data */
	glGenTextures(1, &id);
	//std::cout << "Image Constructed" << std::endl;
}

Texture* Texture::SetData(unsigned char * data) {
	this->Data[0] = data;

	return this;
}

Texture* Texture::SetData(unsigned char * Right, unsigned char * Left, unsigned char * Top, unsigned char * Bottom, unsigned char * Back, unsigned char * Front) {
	this->Data[0] = Right;
	this->Data[1] = Left;
	this->Data[2] = Top;
	this->Data[3] = Bottom;
	this->Data[4] = Back;
	this->Data[5] = Front;

	return this;
}

Texture* Texture::LoadPNG(const char * filePath) {
	int channels;
	this->Data[0] = stbi_load(filePath, &width, &height, &channels, 4);
	std::cout << width << " " << height << std::endl;
	if (Data[0] == nullptr) {
		std::cout << "Unable to load texture" << std::endl;
	}

	return this;
}

Texture* Texture::LoadData() {
	//Bind();

	if (textureType == GL_TEXTURE_CUBE_MAP) {
		/* Load cube map */
		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, pixelDataFromat, pixelDataType, (const void*)Data[i]);
		}
	}
	else {
		/* Load others texture with have only one side */
		if (textureType == GL_TEXTURE_2D_MULTISAMPLE) {
			glTexImage2DMultisample(textureType, 4, internalFormat, width, height, GL_TRUE);
		}
		else {
			glTexImage2D(textureType, 0, internalFormat, width, height, 0, pixelDataFromat, pixelDataType, (const void*)Data[0]);
		}
	}

	//Unbind();
	return this;
}

Texture* Texture::ReloadData() {
	glBindTexture(textureType, id);
	glTexSubImage2D(textureType, 0, 0, 0, width, height, pixelDataFromat, pixelDataType, Data[0]);

	return this;
}

//Set OpenGL texture object parameters.
Texture* Texture::SetParameters() {
	//Activate texture object.
	//Bind();

	glGenerateMipmap(textureType);
	glTexParameteri(textureType, GL_TEXTURE_COMPARE_FUNC, compareFunc);
	glTexParameteri(textureType, GL_TEXTURE_COMPARE_MODE, compareMode);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrap);
	if (textureType == GL_TEXTURE_CUBE_MAP) {
		glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrap);
	}
	glTexParameterf(textureType, GL_TEXTURE_LOD_BIAS, mipMapLevel);
	glTexParameteri(textureType, /*GL_DEPTH_TEXTURE_MODE*/GL_DEPTH_STENCIL_TEXTURE_MODE, dephtMode);// ???
	
	
	/* Anisotropic filtering */
	if (true) {//Temporary
		float Amount = glm::min(antisotropyFilterLevel, (float)GL_MAX_TEXTURE_MAX_ANISOTROPY);
		glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY, Amount);
	}
	else {
		std::cout << "texture filter anisotropic not supported!" << std::endl;
	}

	//Unbind();
	return this;
}

Texture* Texture::Bind() {
	/* Activate buffer for writing data */
	glBindTexture(textureType, id);

	return this;
}

void Texture::Unbind() {
	glBindTexture(textureType, id);
}

//Activate slot for opelGL shader program to know in witch slot to represent loaded data.
//Slot determined in GLSL program by sending slot number into variable.
void Texture::ActivateSlot(int Slot) {
	glActiveTexture(GL_TEXTURE0 + Slot);
	Bind();
}

//Clear data.
void Texture::CleanUp() {
	Unbind();
	glDeleteTextures(1, &id);
	//delete[] Data;
	std::cout << "Image cleared" << std::endl;
}

Texture::~Texture() {
	//std::cout << "Image Deconstructed" << std::endl;
}