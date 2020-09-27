#include "Texture.h"
#include <iostream>

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
}

//Set OpenGL texture object parameters.
void Texture::Setup() {
	//Activate texture object.
	Bind();

	//Set parameters to object but pass no data. So texture object know it's size but not have color data. it will be loaded leater.
	glTexImage2D(textureType, 0, internalFormat, width, height, 0, pixelDataFromat, pixelDataType, NULL);

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
	
	

	/*if (GL.getCapabilities().GL_EXT_texture_filter_anisotropic) {
		float Amount = Math.min(antisotropyFilterLevel, glGetFloat(EXTTextureFilterAnisotropic.GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT));
		glTexParameterf(textureType, EXTTextureFilterAnisotropic.GL_TEXTURE_MAX_ANISOTROPY_EXT, Amount);
	}
	else {
		std::cout << "texture filter anisotropic not supported!" << std::endl;
	}*/

	Unbind();
}

void Texture::Bind() {
	glBindTexture(textureType, id);
}

void Texture::Unbind() {
	glBindTexture(textureType, id);
}

//Activate slot for opelGL shader program to know in witch slot to represent loaded data.
//Slot determined in GLSL program by sending slot number into variable.
/*static void Texture::ActivateSlot(int Slot) {
	glActiveTexture(GL_TEXTURE0 + Slot);
}*/

//Clear data.
void Texture::CleanUp() {
	Unbind();
	glDeleteTextures(1, &id);
	data.clear();
}

Texture::~Texture() {
}