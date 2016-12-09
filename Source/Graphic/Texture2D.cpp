#include "Texture2D.h"

#include <iostream>

Texture2D::Texture2D(
	const std::string _shaderTextureName,
	const std::string path,
	const bool generateMipmaps,
	const int force_channels)
	: shaderTextureSamplerName(_shaderTextureName)
{
	// Load image from disk.
	auto * textureBuffer = SOIL_load_image(path.c_str(), &width, &height, &channels, force_channels);
	if (textureBuffer == nullptr) {
		std::cerr << "ERROR: Failed to load image with path " << path << " into a texture." << std::endl;
		return;
	}
	else {
		std::cout << "- SOIL: Successfully loaded image with path '" << path << "'." << std::endl;
	}

	// Generate texture on GPU.
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Parameter options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload texture buffer.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureBuffer);

	// Mip maps.
	if (generateMipmaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Clean up.
	SOIL_free_image_data(textureBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &textureID);
}

void Texture2D::Activate(int shaderProgram, int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, shaderTextureSamplerName.c_str()), textureUnit);
}