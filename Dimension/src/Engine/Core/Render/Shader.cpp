#include "Shader.h"
#include "GLAD/glad.h"
#include <string>
#include <stdarg.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Dimension::Shader::Shader() {
	this->programID = glCreateProgram();

	addAttribute("Position", 0);
	addAttribute("TextureCoordinates", 1);
	addAttribute("VertextObjectIndex", 2);

	addUniform("diffuseMap");
	//addUniform("textures");
	addUniform("Ocolor");
	addUniform("ModeltransformationArray");
	addUniform("ProjectionView");
	/*addAttribute("Color", 2);
	addAttribute("Normal", 3);
	addAttribute("Tangent", 4);
	addAttribute("BiTangent", 5);
	addAttribute("WeightValue", 6);
	addAttribute("VertexIndex", 7);*/
	
	for (int i = 0; i < 32; i++) {
		addTextures("textures[" + std::to_string(i) + "]");
	}

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
			"in vec3 TextureCoordinates;\n"
			"in int VertextObjectIndex;\n"
			"\n"
			"uniform mat4 Camera;\n"
			"uniform mat4 Projection;\n"
			"uniform mat4 ModeltransformationArray[100];\n"
			"uniform mat4 ProjectionView;\n"
			"out vec3 _TextureCoordinates;\n"
			"flat out int _textureid;\n"
			"\n"
			"void main() {\n"
			"	_TextureCoordinates = TextureCoordinates;\n"
			"	vec4 worldPosition = ProjectionView * ModeltransformationArray[VertextObjectIndex] * vec4(Position, 1.0f);\n"
			"	gl_Position = worldPosition;\n"
			"	_textureid = VertextObjectIndex;\n"
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
									"in vec3 _TextureCoordinates;\n"
									"uniform sampler2D diffuseMap;\n"
									"uniform sampler2D textures[32];\n"
									"uniform vec4 Ocolor;\n"
									"flat in int _textureid;\n"
									"void main() {\n"
										"Pixel = texture(textures[_textureid], _TextureCoordinates.xy) * Ocolor;\n"
									"}";

		code.second.second = (glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(code.second.second, 1, (const char**)&shaderCode, NULL);
		glCompileShader(code.second.second);
		
		Check(code.second.second, GL_COMPILE_STATUS, false, "Shader: Error compiling fragment shader!");

		glAttachShader(programID, code.second.second);
	}

	setAttributes();

	glLinkProgram(programID);


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
/*void Dimension::Shader::addUniform(std::string name, ...) {
	va_list ap;
	va_start(ap, name);
	uniforms[va_arg(ap, std::string)] = -1;//loop throught all names.
	va_end(ap);
	for (std::string _name : name) {
		uniforms[_name] = -1;
	}
}*/
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
		uniforms[Name] = Location;
		if (Location == -1) {
			std::cout << "Uniform " << Name << " " << Location << " setup failed!" << std::endl;
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
	for (std::map<std::string, std::pair<int, int>>::iterator texture = textures.begin(); texture != textures.end(); ++texture){
	//for (std::pair<std::string, std::pair<int, int>> texture : textures) {
		texture->second.first = glGetUniformLocation(programID, texture->first.c_str());//set texture unifrom location.
		texture->second.second = i;//saves texture id for render bind slot.
		glUniform1i(texture->second.first, i);//set texture id.
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

void Dimension::Shader::sendUniform(std::string name, int 			value) {
	glUniform1i(getLocation(name), value);
}
void Dimension::Shader::sendUniform(std::string name, float 		value) {
	glUniform1f(getLocation(name), value);
}
void Dimension::Shader::sendUniform(std::string name, double 		value) {
	glUniform1d(getLocation(name), value);
}
void Dimension::Shader::sendUniform(std::string name, bool 			value) {
	int _value = 0;
	if (value) {
		_value = 1;
	}
	glUniform1i(getLocation(name), _value);
}

void Dimension::Shader::sendUniform(std::string name, glm::vec2 	value) {
	glUniform2f(getLocation(name), value.x, value.y);
}
void Dimension::Shader::sendUniform(std::string name, glm::vec3 	value) {
	glUniform3f(getLocation(name), value.x, value.y, value.z);
}
void Dimension::Shader::sendUniform(std::string name, glm::vec4 	value) {
	glUniform4f(getLocation(name), value.x, value.y, value.z, value.z);
}
void Dimension::Shader::sendUniform(std::string name, glm::mat2 	value) {
	glUniformMatrix2fv(getLocation(name), 1, false, &value[0][0]);
}
void Dimension::Shader::sendUniform(std::string name, glm::mat3 	value) {
	glUniformMatrix3fv(getLocation(name), 1, false, &value[0][0]);
}
void Dimension::Shader::sendUniform(std::string name, glm::mat4 	value) {
	glUniformMatrix4fv(getLocation(name), 1, false, &value[0][0]);
}

void Dimension::Shader::sendUniform(std::string name, int*		value, size_t size) {
	for (int i = 0; i < size; i++) {
		sendUniform(name + "[" + std::to_string(i) + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, float* 	value, size_t size) {
	for (int i = 0; i < size; i++) {
		sendUniform(name + "[" + std::to_string(i) + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, double* 	value, size_t size) {
	for (int i = 0; i < size; i++) {
		sendUniform(name + "[" + std::to_string(i) + "]", value[i]);
	}
}
void Dimension::Shader::sendUniform(std::string name, bool* 	value, size_t size) {
	for (int i = 0; i < size; i++) {
		sendUniform(name + "[" + std::to_string(i) + "]", value[i]);
	}
}

void Dimension::Shader::sendUniform(std::string name, glm::vec2* value, size_t size) {
	glUniform2fv(getLocation(name), size, glm::value_ptr(value[0]));
	
}
void Dimension::Shader::sendUniform(std::string name, glm::vec3* value, size_t size) {
	glUniform3fv(getLocation(name), size, glm::value_ptr(value[0]));
}
void Dimension::Shader::sendUniform(std::string name, glm::vec4* value, size_t size) {
	glUniform4fv(getLocation(name), size, glm::value_ptr(value[0]));
}
void Dimension::Shader::sendUniform(std::string name, glm::mat2* value, size_t size) {
	glUniformMatrix2fv(getLocation(name), size, false, glm::value_ptr(value[0][0]));
}
void Dimension::Shader::sendUniform(std::string name, glm::mat3* value, size_t size) {
	glUniformMatrix3fv(getLocation(name), size, false, glm::value_ptr(value[0][0]));
}
void Dimension::Shader::sendUniform(std::string name, glm::mat4* value, size_t size) {
	glUniformMatrix4fv(getLocation(name), size, false, glm::value_ptr(value[0]));
}

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

/***********************************************************************Private helper functions***********************************************************************/

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