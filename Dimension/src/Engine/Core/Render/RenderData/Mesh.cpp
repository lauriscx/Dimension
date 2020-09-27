#include "Mesh.h"
#include "GLAD/glad.h"

Mesh::Mesh() : vertexCount(0) {
}

void Mesh::GenTriangle(float width, float height) {
	unsigned int indices[] = {0, 1, 2};
	vertexCount = sizeof(indices) / sizeof(unsigned int);

	float positions[] = {
						-width/2.0f, -height / 2.0f, 0.0f,
						width / 2.0f, -height / 2.0f, 0.0f,
						0.0f, height / 2.0f, 0.0f
						};


	vao.Bind();

	VBO* ibo_indices = new VBO();
	ibo_indices->setBufferType(GL_ELEMENT_ARRAY_BUFFER);
	ibo_indices->setStorageType(GL_STATIC_DRAW);
	ibo_indices->setDataType(GL_UNSIGNED_INT);

	ibo_indices->bind();
	ibo_indices->StoreData(indices, sizeof(indices));

	vbos.push_back(ibo_indices);

	VBO* vbo_position = new VBO();

	vbo_position->setBufferType(GL_ARRAY_BUFFER);
	vbo_position->setStorageType(GL_STATIC_DRAW);
	vbo_position->setDataType(GL_FLOAT);
	vbo_position->setLocation(0);
	vbo_position->setSize(3);

	vbo_position->bind();
	vbo_position->StoreData(positions, sizeof(positions));
	vbo_position->AttributeSetup();

	vbos.push_back(vbo_position);
}

void Mesh::GenRectangle(float width, float height) {
	int indices[] = {0, 1, 2, 3, 1, 0};
	vertexCount = sizeof(indices) / sizeof(int);
	float positions[] = {
			-width / 2.0f, -height / 2.0f, 0.0f,
			-width, height / 2.0f, 0.0f,
			width / 2.0f, height / 2.0f, 0.0f,
			width / 2.0f, -height / 2.0f, 0.0f

	};

	vao.Bind();
	/*VBO ibo_indices;
	ibo_indices.setBufferType(GL_ELEMENT_ARRAY_BUFFER);
	ibo_indices.setStorageType(GL_STATIC_DRAW);
	ibo_indices.setDataType(GL_UNSIGNED_INT);

	ibo_indices.bind();
	ibo_indices.StoreData(indices);

	vbos.push_back(ibo_indices);*/

	VBO* vbo_position = new VBO();

	vbo_position->setBufferType(GL_ARRAY_BUFFER);
	vbo_position->setStorageType(GL_STATIC_DRAW);
	vbo_position->setDataType(GL_FLOAT);
	vbo_position->setLocation(0);
	vbo_position->setSize(3);

	vbo_position->bind();
	vbo_position->AttributeSetup();
	vbo_position->StoreData(positions, sizeof(positions) / sizeof(int));

	vbos.push_back(vbo_position);
}

//Load data from file.
void Mesh::Load() {

	//Create objects.
/*	vao = new VAO();
	vao.Bind();
	List<VBO> vbos = new ArrayList<VBO>();

	//Creates VBO object and fill with data using builder class witch is integrated in vbo class.
	//Indices
	vbos.add(new VBO.Builder().setBufferType(GL_ELEMENT_ARRAY_BUFFER)
		.setStorageType(GL_STATIC_DRAW)
		.setDataType(GL_UNSIGNED_INT)
		.build()
	);
	//Activate vbo
	vbos.get(0).bind();
	//Fill vbo with data.
	vbos.get(0).StoreData(objectFile.getIndex());

	//Positions
	vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
		.setStorageType(GL_STATIC_DRAW)
		.setDataType(GL_FLOAT)
		.setLocation(0)
		.setSize(3)
		.build()
	);
	vbos.get(1).bind();
	vbos.get(1).AttributeSetup();
	vbos.get(1).StoreData(objectFile.getPositions());

	//UVs
	vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
		.setStorageType(GL_STATIC_DRAW)
		.setDataType(GL_FLOAT)
		.setLocation(1)
		.setSize(2)
		.build()
	);
	vbos.get(2).bind();
	vbos.get(2).AttributeSetup();
	vbos.get(2).StoreData(objectFile.getUVs());


	//Colors
	vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
		.setStorageType(GL_STATIC_DRAW)
		.setDataType(GL_FLOAT)
		.setLocation(2)
		.setSize(2)
		.build()
	);
	vbos.get(3).bind();
	vbos.get(3).AttributeSetup();
	vbos.get(3).StoreData(objectFile.getColors());

	//Normals
	vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
		.setStorageType(GL_STATIC_DRAW)
		.setDataType(GL_FLOAT)
		.setLocation(3)
		.setSize(3)
		.build()
	);
	vbos.get(4).bind();
	vbos.get(4).AttributeSetup();
	vbos.get(4).StoreData(objectFile.getNormals());

	//Tangents
	vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
		.setStorageType(GL_STATIC_DRAW)
		.setDataType(GL_FLOAT)
		.setLocation(4)
		.setSize(3)
		.build()
	);
	vbos.get(5).bind();
	vbos.get(5).AttributeSetup();
	vbos.get(5).StoreData(objectFile.getTangents());

	//BiTangents
	vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
		.setStorageType(GL_STATIC_DRAW)
		.setDataType(GL_FLOAT)
		.setLocation(5)
		.setSize(3)
		.build()
	);
	vbos.get(6).bind();
	vbos.get(6).AttributeSetup();
	vbos.get(6).StoreData(objectFile.getBiTangents());

	//Object has animation load additional data.
	if (objectFile.getWeights().length != 0) {
		//Joint weights
		vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
			.setStorageType(GL_STATIC_DRAW)
			.setDataType(GL_FLOAT)
			.setLocation(6)
			.setSize(4)
			.build()
		);
		vbos.get(7).bind();
		vbos.get(7).AttributeSetup();
		vbos.get(7).StoreData(objectFile.getWeights());

		//joint Index
		vbos.add(new VBO.Builder().setBufferType(GL_ARRAY_BUFFER)
			.setStorageType(GL_STATIC_DRAW)
			.setDataType(GL_INT)
			.setLocation(7)
			.setSize(4)
			.build()
		);
		vbos.get(8).bind();
		vbos.get(8).AttributeISetup();//AttribIPointer.
		vbos.get(8).StoreData(objectFile.getBonesIndex());
	}
	//Close VAO
	vao.Unbind();*/
}

//Get vao ID.
VAO Mesh::GetVAO() {
	return vao;
}
//Get VBO list.
std::vector<VBO*> Mesh::GetVBOs() {
	return vbos;
}

//Get vertex count of mesh.
int Mesh::getVertexCount() {
	return vertexCount;//objectFile.getPositions().length;
}

//Clear object data from memory.
void Mesh::clean() {
	//Disable VAO if active.
	//vao.Bind();
	//Call function to clear objects they own data.
	/*for (VBO vbo : vbos) {
		vbo.CleanUp();
	}
	vao.CleanUp();*/
}
Mesh::~Mesh()
{
}