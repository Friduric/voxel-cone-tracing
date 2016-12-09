#include "OrthographicCamera.h"
OrthographicCamera::OrthographicCamera() : Camera(glm::ortho(-1, 1, -1, 1, -1, 1)) {}
OrthographicCamera::OrthographicCamera(float aspect, float near, float far) : Camera(glm::ortho(-1.0f, 1.0f, -aspect, aspect, near, far)) {}
