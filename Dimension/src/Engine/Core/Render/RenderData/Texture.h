#include <glad/glad.h>
#include <vector>


class Texture {
public:
	Texture();

	Texture* SetData(unsigned char* data);
	Texture* SetData(unsigned char * Right, unsigned char * Left, unsigned char * Top, unsigned char * Bottom, unsigned char * Back, unsigned char * Front);

	Texture* LoadPNG(const char* filePath);

	Texture* LoadData();
	Texture* ReloadData();
	Texture* SetParameters();
	Texture* Bind();
	void Unbind();

	void ActivateSlot(int Slot);
	void CleanUp();

	//void ActivateSlot(int Slot);

	~Texture();

private:
	/* Texture size. */
	int width;
	int height;

	/* Texture data */
	unsigned char * Data[6] = { 0 };

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