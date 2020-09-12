#pragma once

/* Completed */
class VAO {
public:
	VAO();
	
	void Bind();//Activate object.
	void Unbind();//Disable object.
	void CleanUp();//Clean up data.

	~VAO();

private:
	//VAO id returned by openGL.
	unsigned int VAOID;
};