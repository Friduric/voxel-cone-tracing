#pragma once;

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm\glm.hpp>

class Shader;

/// <summary> Represents a material that references a gl program, textures and settings. </summary>
class Material {
public:
	~Material();
	Material(std::string _name,
		Shader * vertexShader,
		Shader * fragmentShader,
		Shader * geometryShader = nullptr,
		Shader * tessEvaluationShader = nullptr,
		Shader * tessControlShader = nullptr);

	/// <summary> The actual OpenGL / GLSL program identifier. </summary>
	GLuint program;

	/// <summary> A name. Just an identifier. Doesn't do anything practical. </summary>
	std::string name;
};