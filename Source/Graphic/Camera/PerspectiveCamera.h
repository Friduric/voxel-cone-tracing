#pragma once

#include "Camera.h"

/// <summary> A perspective camera. </summary>
class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float _fov = 0.7, float _aspect = 1.0, float _near = 0.1, float _far = 500);
};