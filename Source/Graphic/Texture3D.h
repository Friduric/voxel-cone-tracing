#pragma once

#include <vector>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <SOIL\SOIL.h>

/// <summary> A 3D texture wrapper class. Handles important OpenGL calls. </summary>
class Texture3D {
public:
	unsigned char * textureBuffer = nullptr;
	GLuint textureID;

	/// <summary> Activates this texture and passes it on to a texture unit on the GPU. </summary>
	void Activate(const int shaderProgram, const std::string glSamplerName, const int textureUnit = GL_TEXTURE0);

	/// <summary> Clears this texture using a given clear color. </summary>
	void Clear(GLfloat clearColor[4]);

	Texture3D(
		const std::vector<GLfloat> & textureBuffer,
		const int width, const int height, const int depth,
		const bool generateMipmaps = true
	);
private:
	int width, height, depth;
	std::vector<GLfloat> clearData;
};