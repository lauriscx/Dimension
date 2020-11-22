#pragma once

class VBO {
public:
	VBO();
	//Activate buffer.
	void bind();

	void setStorageType	(int storageType);
	void setNormalized	(bool normalized);
	void setBufferType	(int bufferType);
	void setDataType	(int dataType);
	void setLocation	(int location);
	void setStride		(int stride);
	void setOffset		(int offset);
	void setSize		(int size);

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
	void StoreData(int* data, int _size);
	void StoreData(unsigned int* data, int _size);
	void StoreData(float* data, int _size);
	void StoreData(double* data);

	void StoreData(long* Data, int size, int Offset);
	void StoreData(int* Data, int size, int Offset);
	void StoreData(float* Data, int size, int Offset);
	void StoreData(double* Data, int size, int Offset);
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
