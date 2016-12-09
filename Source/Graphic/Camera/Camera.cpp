#include "Camera.h"

const glm::mat4 & Camera::getProjectionMatrix() const {
	return projectionMatrix;
}

void Camera::setProjectionMatrix(glm::mat4 _projectionMatrix) {
	projectionMatrix = projectionMatrix;
	projectionMatrixHasChanged = true;
	dirtyProjectionMatrix = true;
}

void Camera::updateViewMatrix() {
	viewMatrix = glm::lookAt(position, position + rotation, up);
}
