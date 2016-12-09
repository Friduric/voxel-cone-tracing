#pragma once

#include <vector>

#include "VertexData.h"

/// <summary> Represents a basic mesh with OpenGL related attributes (vertex data, indices), 
/// and variables (VAO, VAO, and EBO identifiers). </summary>
class Mesh {
public:
	/// <summary> If the mesh is static, i.e. does not change over time, set this to true to improve performance. </summary>
	bool staticMesh = true;

	Mesh();
	~Mesh();

	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indices;

	// Used for (shared) rendering.
	int program;
	unsigned int vbo, vao, ebo; // Vertex Buffer Object, Vertex Array Object, Element Buffer Object.
	bool meshUploaded = false;
private:
	static unsigned int idCounter;
};