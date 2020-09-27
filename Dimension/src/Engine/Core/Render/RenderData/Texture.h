#include <glad/glad.h>
#include <vector>


class Texture {
public:
	Texture();

	void Setup();
	void Bind();
	void Unbind();

	void ActivateSlot(int Slot);
	void CleanUp();

	~Texture();

private:
	/* Texture size. */
	int width;
	int height;

	//Texture color data. */
	std::vector<unsigned int> data;

	/* Texture ID generate by OpenGL. */
	unsigned int id;

	/* Texture parameters data. */
	int 	textureType;
	int 	filter;
	int 	wrap;
	float 	mipMapLevel;
	int 	internalFormat;
	int 	pixelDataFromat;
	float 	antisotropyFilterLevel;
	int 	compareFunc;
	int 	compareMode;
	int 	dephtMode;
	int 	pixelDataType;
};