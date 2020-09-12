#include "VAO.h"
#include "VBO.h"
#include <vector>

class Mesh {
public:
	Mesh();

	void Load();//Load data from file.
	VAO GetVAO();//Get vao ID.
	std::vector<VBO> GetVBOs();//Get VBO list.
	int getVertexCount();//Get vertex count of mesh.
	void clean();//Clear object data from memory.

	~Mesh();

private:
	//Vertex array object store all vertex buffer objects (vbo)
	VAO vao;
	//Save data in vbo about 3D or 2D object.
	std::vector<VBO> vbos;
	//Used to load data from file.
	//ObjectFile	objectFile;
};