#include "Shader.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

GLuint Shader::compile() {
	// Create and compile shader.
	GLuint id = glCreateShader(shaderType);
	const char * source = rawShader.c_str();
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	// Check if we succeeded.
	std::string typeName = " (" + GetShaderTypeName() + ")";
	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar log[1024];
		glGetShaderInfoLog(id, 1024, nullptr, log);
		std::cerr << "- Failed to compile shader '" << path << "' : " << shaderType << typeName << "!" << std::endl;
		std::cerr << "LOG: " << std::endl << log << std::endl;
		std::getchar();
		return 0;
	}
	if (id == 0) {
		std::cerr << "- Could not compile shader '" << path << "' : " << shaderType << typeName << "!" << std::endl;
		std::getchar();
		return 0;
	}
	std::cout << "- Shader '" << path << "' : " << shaderType << typeName << " compiled successfully." << std::endl;
	return id;
}

Shader::Shader(std::string _path, ShaderType _type) : path(_path), shaderType(_type) {
	// Load the shader instantly.
	std::ifstream fileStream(path, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Couldn't load shader '" + std::string(path) + "'." << std::endl;
		fileStream.close();
		return;
	}
	std::string line = "";
	rawShader = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		rawShader.append(line + "\n");
	}
	fileStream.close();
}

std::string Shader::GetShaderTypeName()
{
	switch (shaderType) {
	case ShaderType::FRAGMENT:					return "fragment";
	case ShaderType::VERTEX:					return "vertex";
	case ShaderType::GEOMETRY:					return "geometry";
	case ShaderType::TESSELATION_CONTROL:		return "tesselation control";
	case ShaderType::TESSELATION_EVALUATION:	return "tesselation evaluation";
	default:									return "unknown";
	}
}