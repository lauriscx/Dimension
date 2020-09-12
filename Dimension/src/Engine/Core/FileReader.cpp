#include "FileReader.h"
#include <iostream>
#include <fstream>

FileReader::FileReader()
{
}

std::string FileReader::ReadFile(std::string FilePath) {
	std::ifstream ShaderFile(FilePath);
	std::string Data;
	if (ShaderFile.good()) {
		Data = std::string(std::istreambuf_iterator<char>(ShaderFile), std::istreambuf_iterator<char>());
	}
	ShaderFile.close();
	return Data;
}

FileReader::~FileReader()
{
}
