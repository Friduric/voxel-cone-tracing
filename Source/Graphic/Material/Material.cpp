#include "Material.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <gtc/type_ptr.hpp>

#include "Shader.h"

Material::~Material()
{
	glDeleteProgram(program);
}

Material::Material(
	std::string _name,
	Shader * vertexShader,
	Shader * fragmentShader,
	Shader * geometryShader,
	Shader * tessEvaluationShader,
	Shader * tessControlShader) : name(_name)
{
	assert(vertexShader != nullptr);
	assert(fragmentShader != nullptr);

	GLuint vertexShaderID, fragmentShaderID, geometryShaderID, tessEvaluationShaderID, tessControlShaderID;
	program = glCreateProgram();

	// Vertex shader.
	assert(vertexShader->shaderType == Shader::ShaderType::VERTEX);
	vertexShaderID = vertexShader->compile();
	glAttachShader(program, vertexShaderID);

	// Fragment shader.
	assert(fragmentShader->shaderType == Shader::ShaderType::FRAGMENT);
	fragmentShaderID = fragmentShader->compile();
	glAttachShader(program, fragmentShaderID);

	// Geometry shader.
	if (geometryShader != nullptr) {
		assert(geometryShader->shaderType == Shader::ShaderType::GEOMETRY);
		geometryShaderID = geometryShader->compile();
		glAttachShader(program, geometryShaderID);
	}

	// Tesselation evaluation shader.
	if (tessEvaluationShader != nullptr) {
		assert(tessEvaluationShader->shaderType == Shader::ShaderType::TESSELATION_EVALUATION);
		tessEvaluationShaderID = tessEvaluationShader->compile();
		glAttachShader(program, tessEvaluationShaderID);
	}

	// Tesselation control shader.
	if (tessControlShader != nullptr) {
		assert(tessControlShader->shaderType == Shader::ShaderType::TESSELATION_CONTROL);
		tessControlShaderID = tessControlShader->compile();
		glAttachShader(program, tessControlShaderID);
	}

	glLinkProgram(program);

	// Check if we succeeded.
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar log[1024];
		glGetProgramInfoLog(program, 1024, nullptr, log);
		std::cerr << "- Failed to link program and material '" << name << "' (" << program << ")." << std::endl;
		std::cerr << "LOG: " << std::endl << log << std::endl;
	}
	else {
		std::cout << "- Material '" << name << "' (program " << program << ") sucessfully created." << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	if (geometryShader != nullptr) { glDeleteShader(geometryShaderID); }
	if (tessControlShader != nullptr) { glDeleteShader(tessControlShaderID); }
	if (tessEvaluationShader != nullptr) { glDeleteShader(tessEvaluationShaderID); }
}