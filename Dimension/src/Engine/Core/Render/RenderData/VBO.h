#pragma once

class VBO {
public:
	VBO();
	//Activate buffer.
	void bind();

	//Disable buffer.
	void unbind();

	//Enable attribute array by index witch is set in shader class.
	void enableAttributeArray();
	//Disable attribute array by index witch is set in shader class.
	void disableAttributeArray();


	void AttribDivisor(int Incrament);
	void AttributeSetup();
	void AttributeISetup();
	/*Reserve data in buffer for latter to add*/
	void ReserveData(int Size);

	/*Store data functions*/
	void StoreData(long* data);
	void StoreData(int* data);
	void StoreData(float* data);
	void StoreData(double* data);

	void StoreData(long* Data, int Offset);
	void StoreData(int* Data, int Offset);
	void StoreData(float* Data, int Offset);
	void StoreData(double* Data, int Offset);
	//Clean up data
	void CleanUp();
	~VBO();

private:
	//vertex buffer object ID.
	unsigned int id;
	//buffer parameters.
	int bufferType;
	int storageType;
	int dataType;
	int location;
	int stride;
	int offset;
	int size;
	bool normalized;
};
