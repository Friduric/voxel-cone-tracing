// Author:	Fredrik Präntare <prantare@gmail.com> (student at Linköping University)
// Date:	11/26/2016
#include "StandardShapes.h"

#include <vector>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "Mesh.h"

/// <summary> Creates a quad </summary>
Mesh StandardShapes::createQuad() {
	Mesh emptyMesh;

	VertexData v;

	v.position = { -1, -1, 1 };
	v.texCoord = { 0, 1 };
	v.normal = { 0, 0, 1 };
	emptyMesh.vertexData.push_back(v);

	v.position = { 1, -1, 1 };
	v.texCoord = { 0, 0 };
	v.normal = { 0, 0, 1 };
	emptyMesh.vertexData.push_back(v);

	v.position = { 1, 1, 1 };
	v.texCoord = { 1, 1 };
	v.normal = { 0, 0, 1 };
	emptyMesh.vertexData.push_back(v);

	v.position = { -1, 1, 1 };
	v.texCoord = { 1 , 0 };
	v.normal = { 0, 0, 1 };
	emptyMesh.vertexData.push_back(v);

	emptyMesh.indices = { 0, 1, 2, 0, 2, 3 };

	return emptyMesh;
}

/// <summary> Creates a 2x2x2 cube centered at origin. </summary>
Mesh StandardShapes::createCube() {
	Mesh emptyMesh;

	emptyMesh.vertexData.push_back(VertexData({ -1, -1, 1 }, { 0.5, 1, 1 }));
	emptyMesh.vertexData.push_back(VertexData({ 1, -1, 1 }, { 1, 0.5, 1 }));
	emptyMesh.vertexData.push_back(VertexData({ 1, 1, 1 }, { 1, 0, 0 }));
	emptyMesh.vertexData.push_back(VertexData({ -1, 1, 1 }, { 0, 1, 1 }));
	emptyMesh.vertexData.push_back(VertexData({ -1, -1, -1 }, { 1, 1, 1 }));
	emptyMesh.vertexData.push_back(VertexData({ 1, -1, -1 }, { 1, 1, 1 }));
	emptyMesh.vertexData.push_back(VertexData({ 1, 1, -1 }, { 1, 1, 1 }));
	emptyMesh.vertexData.push_back(VertexData({ -1, 1, -1 }, { 1, 0, 1 }));
	emptyMesh.indices = {
		0, 1, 2, 2, 3, 0, 3, 2, 6,
		6, 7, 3, 7, 6, 5, 5, 4, 7,
		4, 5, 1, 1, 0, 4, 4, 0, 3,
		3, 7, 4, 1, 5, 6, 6, 2, 1
	};
	emptyMesh.staticMesh = false;

	return emptyMesh;
}
