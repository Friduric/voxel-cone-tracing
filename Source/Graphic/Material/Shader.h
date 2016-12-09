#pragma once

#include <string>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

/// <summary> Represents a shader program. </summary>
class Shader {
public:
	enum ShaderType {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
		TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER
	};

	ShaderType shaderType;

	/// <summary> Returns the name of the shader type of this shader. </summary>
	std::string GetShaderTypeName();

	/// <summary> The shader path. </summary>
	std::string path;

	/// <summary> Compiles the shader. Returns the OpenGL shader ID. </summary>
	GLuint compile();

	/// <summary> Creates and loads a shader from disk. Does not compile it. </summary>
	Shader(std::string path, ShaderType shaderType);
private:
	std::string rawShader;
	Shader();
};