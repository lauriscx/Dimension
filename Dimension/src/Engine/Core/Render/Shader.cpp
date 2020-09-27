#include "Shader.h"
#include "GLAD/glad.h"
#include <string>
#include <stdarg.h>
#include <iostream>

Dimension::Shader::Shader() {
	this->programID = glCreateProgram();

	addAttribute("Position", 0);
	/*addAttribute("UV", 1);
	addAttribute("Color", 2);
	addAttribute("Normal", 3);
	addAttribute("Tangent", 4);
	addAttribute("BiTangent", 5);
	addAttribute("WeightValue", 6);
	addAttribute("VertexIndex", 7);*/
}

void Dimension::Shader::addVertexCode(std::string name, std::string code) {
	//code = interpretator.Interpretator(code);
	//parseData(code);
	vertex[name] = std::pair<std::string, int>(code, -1);
}
void Dimension::Shader::addGeometryCode(std::string name, std::string code) {
	//code = interpretator.Interpretator(code);
	//parseData(code);
	geometry[name] = std::pair<std::string, int>(code, -1);
}
void Dimension::Shader::addFragmentCode(std::string name, std::string code) {
	//code = interpretator.Interpretator(code);
	//parseData(code);
	fragment[name] = std::pair<std::string, int>(code, -1);
}

int 	Dimension::Shader::getID() {
	return this->ID;
}
int 	Dimension::Shader::program() {
	return this->programID;
}
void Dimension::Shader::compile() {
	for (std::pair<std::string, std::pair<std::string, unsigned int>> code : vertex) {

		const char* shaderCode = "#version 430\n"
			"in vec3 Position;\n"
			"\n"
			"uniform mat4 ModelTransformation;\n"
			"uniform mat4 Camera;\n"
			"uniform mat4 Projection;\n"
			"\n"
			"void main() {\n"
			"	//vec4 worldPosition = ModelTransformation * vec4(Position, 1.0f);\n"
			"	gl_Position = vec4(Position, 1.0f);\n"
			"}";

		code.second.second = (glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(code.second.second, 1, (const char**)&shaderCode, NULL);
		glCompileShader(code.second.second);

		Check(code.second.second, GL_COMPILE_STATUS, false, "Shader: Error compiling vertex shader!");

		glAttachShader(programID, code.second.second);
	}

	for (std::pair<std::string, std::pair<std::string, unsigned int>> code : geometry) {
		code.second.second = (glCreateShader(GL_GEOMETRY_SHADER));
		glShaderSource(code.second.second, 1, (const char**)&code.second.first, NULL);
		glCompileShader(code.second.second);

		Check(code.second.second, GL_COMPILE_STATUS, false, "Shader: Error compiling geometry shader!");

		glAttachShader(programID, code.second.second);
	}

	for (std::pair<std::string, std::pair<std::string, unsigned int>> code : fragment) {
		const char* shaderCode =	"#version 430\n"
									"out vec4 Pixel;\n"
									"void main() {\n"
										"Pixel = vec4(0.0, 0.5, 0.7, 1);// * (texture2D(DiffuseMap, Paralax) + material.color);\n"
									"}";

		code.second.second = (glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(code.second.second, 1, (const char**)&shaderCode, NULL);
		glCompileShader(code.second.second);
		
		Check(code.second.second, GL_COMPILE_STATUS, false, "Shader: Error compiling fragment shader!");

		glAttachShader(programID, code.second.second);
	}

	setAttributes();

	glLinkProgram(programID);
	int result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if (result == -1) {
		int a = 5;
		//td::_Count_pr << "Shader: " + programID + "\ncompaling error: " + glGetProgramInfoLog(programID, glGetProgramiv(programID, GL_INFO_LOG_LENGTH));
	}


	glValidateProgram(programID);

	setupUniforms();
	setupTextures();
}
void Dimension::Shader::start() {
	glUseProgram(programID);
}
void Dimension::Shader::stop() {
	glUseProgram(0);
}

void Dimension::Shader::update() {
	start();
	/*sendUniformPointLight("light", LightsLists.pointLights());
	sendUniformDirectionLight("Dirlight", LightsLists.directionLights());
	sendUniformSpotLight("Spotlight", LightsLists.spotLights());*/

	/*sendUniform("PointLightNumber", LightsLists.pointLights().size());
	sendUniform("DirectionLightNumber", LightsLists.directionLights().size());
	sendUniform("SpotLightNumber", LightsLists.spotLights().size());*/
}

void Dimension::Shader::addAttribute(std::string name, int value) {
	attributes[name] = value;
}
void Dimension::Shader::setAttributes() {
	for (std::pair<std::string, int> attribute : attributes) {
		glBindAttribLocation(this->programID, attribute.second, attribute.first.c_str());
	}
}

void Dimension::Shader::addUniform(std::string name) {
	uniforms[name] = -1;
}
void Dimension::Shader::addUniform(std::string name, ...) {
	va_list ap;
	va_start(ap, name);
	uniforms[va_arg(ap, std::string)] = -1;//loop throught all names.
	va_end(ap);
	/*for (std::string _name : name) {
		uniforms[_name] = -1;
	}*/
}
/*
void Dimension::Shader::AddStructureUniform(std::string name, std::string variables, ...) {
	for (std::string _name : variables) {
		uniforms[name + "." + _name] = -1;
	}
}*/

void Dimension::Shader::AddUniformArray(std::string name, int from, int to) {
	for (int i = from; i < to; i++) {
		uniforms[name + "[" + std::to_string(i)  + "]"] = -1;
	}
}
/*void Dimension::Shader::AddStructureUniformArray(std::string name, int from, int to, std::string variables, ...) {
	for (int i = from; i < to; i++) {
		for (std::string _name : variables) {
			uniforms[name + "[" + std::to_string(i) + "]." + _name] = -1;
		}
	}
}*/
void Dimension::Shader::setupUniforms() {
	start();
	for (std::pair<std::string, int> entry : uniforms) {
		std::string Name = entry.first;
		int Location = glGetUniformLocation(programID, Name.c_str());
		entry.first = Location;
		if (Location == -1) {
			//System.err.println("Uniform " + Name + " " + Location + " setup failed!");
		}
	}
	stop();
}

void Dimension::Shader::addTextures(std::string names, ...) {
	//for (std::string name : names) {
		textures[names] = std::pair<int, int>(-1, 0);
	//}
}
void Dimension::Shader::setupTextures() {
	start();
	int i = 0;
	for (std::pair<std::string, std::pair<int, int>> texture : textures) {
		texture.second.first = glGetUniformLocation(programID, texture.first.c_str());//set texture unifrom location.
		texture.second.second = i;//saves texture id for render bind slot.
		glUniform1i(texture.second.first, i);//set texture id.
		i++;
	}
	stop();
}
int 	Dimension::Shader::getTextureSlot(std::string name) {
	return textures[name].second;
}

void Dimension::Shader::addFragment(std::string name, int index) {
	fragments[name] = index;
}
void Dimension::Shader::setupFragments() {
	for (std::pair<std::string, int> fragment : fragments) {
		glBindFragDataLocation(programID, 3, fragment.first.c_str());
	}
}

/* Implement uniform blocks */
void Dimension::Shader::AddUniformBlock(std::string name, int index, int size) {
}
void Dimension::Shader::SetUniformBlockIndex(std::string name, int index) {
}
void Dimension::Shader::SetUniformBlockSize(std::string name, int size) {
}
void Dimension::Shader::SetUpUniformBlocks() {
	start();
	//System.err.println("Not implemented yet!");
	stop();
}
void Dimension::Shader::SendUniformBlocks(std::string name, void* data) {
}

int Dimension::Shader::getLocation(std::string name) {
	if (uniforms[name] != 0) {
		return uniforms[name];
	}
	return 0;
}

void Dimension::Shader::Check(unsigned int& Shader, unsigned int Flag, bool IsProgram, const std::string& ErrorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	IsProgram ? glGetProgramiv(Shader, Flag, &success) : glGetShaderiv(Shader, Flag, &success);

	if (success == GL_FALSE) {
		IsProgram ? glGetProgramInfoLog(Shader, sizeof(error), NULL, error) : glGetShaderInfoLog(Shader, sizeof(error), NULL, error);
		std::cout << ErrorMessage << " : " << error << "!\n";
	}
}

void Dimension::Shader::sendUniform(std::string name, int 		value) {
	glUniform1i(getLocation(name), value);
}
void Dimension::Shader::sendUniform(std::string name, float 		value) {
	glUniform1f(getLocation(name), value);
}
void Dimension::Shader::sendUniform(std::string name, double 	value) {
	glUniform1d(getLocation(name), value);
}
void Dimension::Shader::sendUniform(std::string name, bool 	value) {
	int _value = 0;
	if (value) {
		_value = 1;
	}
	glUniform1i(getLocation(name), _value);
}
/*
void Dimension::Shader::sendUniform(std::string name, Vector2f 	value) {
	glUniform2f(getLocation(name), value.x, value.y);
}
void Dimension::Shader::sendUniform(std::string name, Vector3f 	value) {
	glUniform3f(getLocation(name), value.x, value.y, value.z);
}
void Dimension::Shader::sendUniform(std::string name, Vector4f 	value) {
	glUniform4f(getLocation(name), value.x, value.y, value.z, value.w);
}
void Dimension::Shader::sendUniform(std::string name, Matrix2f 	value) {
	float[] data = new float[2 * 2];
	value.get(data);
	glUniformMatrix2fv(getLocation(name), false, data);
}
void Dimension::Shader::sendUniform(std::string name, Matrix3f 	value) {
	float[] data = new float[3 * 3];
	value.get(data);
	//glUniformMatrix3fv(getLocation(name), false, MatrixBuffer3);

}
void Dimension::Shader::sendUniform(std::string name, Matrix4f 	value) {
	float[] data = new float[4 * 4];
	value.get(data);
	glUniformMatrix4fv(getLocation(name), false, data);
}*/
void Dimension::Shader::sendUniform(std::string name, int*		value) {
	/*for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}*/
}
void Dimension::Shader::sendUniform(std::string name, float* 	value) {
	/*for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}*/
}
void Dimension::Shader::sendUniform(std::string name, double* 	value) {
	/*for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}*/
}
void Dimension::Shader::sendUniform(std::string name, bool* 	value) {
	/*for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}*/
}
/*
void Dimension::Shader::sendUniform(std::string name, Vector2f[] value) {
	for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, Vector3f[] value) {
	for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, Vector4f[] value) {
	for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, Matrix2f[] value) {
	for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, Matrix3f[] value) {
	for (int i = 0; i < value.length; i++) {
		sendUniform(name + "[" + i + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, Matrix4f[] value) {
	for (int i = 0; i < value.length; i++) {
		if (value[i] != null) {
			sendUniform(name + "[" + i + "]", value[i]);
		}
	}
}*/

/*
void Dimension::Shader::sendUniformDirectionLight(std::string name, List<DirectionLight> value) {
	for (int i = 0; i < value.size(); i++) {
		if (value.get(i) != null) {
			sendUniform(name + "[" + i + "].light.intensivity", value.get(i).intensivity());
			sendUniform(name + "[" + i + "].light.color", value.get(i).color());
			sendUniform(name + "[" + i + "].direction", value.get(i).Direction());
		}
	}
}
void Dimension::Shader::sendUniformPointLight(std::string name, List<PointLight> 	value) {
	for (int i = 0; i < value.size(); i++) {
		if (value.get(i) != null) {
			sendUniform(name + "[" + i + "].baselight.intensivity", value.get(i).intensivity());
			sendUniform(name + "[" + i + "].constant", value.get(i).attenuationConstant());
			sendUniform(name + "[" + i + "].linear", value.get(i).attenuationLinear());
			sendUniform(name + "[" + i + "].quadratic", value.get(i).attenuationQuadratic());
			sendUniform(name + "[" + i + "].position", value.get(i).Position());
			sendUniform(name + "[" + i + "].baselight.color", value.get(i).color());
			sendUniform(name + "[" + i + "].range", value.get(i).Range());
		}
	}
}
void Dimension::Shader::sendUniformSpotLight(std::string name, List<SpotLight> 	value) {
	for (int i = 0; i < value.size(); i++) {
		if (value.get(i) != null) {
			sendUniform(name + "[" + i + "].intensivity", value.get(i).intensivity());
			sendUniform(name + "[" + i + "].color", value.get(i).color());
		}
	}
}*/

void Dimension::Shader::cleanUp() {
	stop();

	for (std::pair<std::string, std::pair<std::string, int>> code : vertex) {
		glDetachShader(this->programID, code.second.second);
		glDeleteShader(code.second.second);
	}

	for (std::pair<std::string, std::pair<std::string, int>> code : geometry) {
		glDetachShader(this->programID, code.second.second);
		glDeleteShader(code.second.second);
	}

	for (std::pair<std::string, std::pair<std::string, int>> code : fragment) {
		glDetachShader(this->programID, code.second.second);
		glDeleteShader(code.second.second);
	}

	glDeleteProgram(this->programID);
}




Dimension::Shader::~Shader() {
}
