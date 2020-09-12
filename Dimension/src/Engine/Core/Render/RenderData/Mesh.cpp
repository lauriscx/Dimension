#include "Mesh.h"

Mesh::Mesh() {
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
std::vector<VBO> Mesh::GetVBOs() {
	return vbos;
}

//Get vertex count of mesh.
int Mesh::getVertexCount() {
	return 0;//objectFile.getPositions().length;
}

//Clear object data from memory.
void Mesh::clean() {
	//Disable VAO if active.
	vao.Bind();
	//Call function to clear objects they own data.
	for (VBO vbo : vbos) {
		vbo.CleanUp();
	}
	vao.CleanUp();
}
Mesh::~Mesh()
{
}