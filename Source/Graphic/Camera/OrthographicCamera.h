#pragma once

#include "Camera.h"

/// <summary> An orthographic camera. </summary>
class OrthographicCamera : public Camera {
public:
	OrthographicCamera();
	OrthographicCamera(float aspect, float near = -100.0f, float far = 100.0f);
};