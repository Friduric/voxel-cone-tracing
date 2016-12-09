#pragma once

#include <vector>

#include "Transform.h"
#include "Mesh.h"

/// <summary> A 'concatenation' of several meshes. </summary>
class Shape {
public:
	std::vector<Mesh> meshes;
	Shape() {}
	~Shape() {}
};