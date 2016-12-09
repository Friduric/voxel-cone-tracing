#pragma once

#include "gtc\matrix_transform.hpp"

/// <summary> A camera base class. </summary>
class Camera {
public:
	glm::vec3 up = { 0,1,0 }, rotation = { 0,0,-1 }, position = { 0,0,0 };
	glm::mat4 viewMatrix;
	const glm::mat4 & getProjectionMatrix() const;
	void setProjectionMatrix(glm::mat4 projectionMatrix);
	virtual void updateViewMatrix();
	glm::vec3 right() { return glm::normalize(-glm::cross(up, rotation)); }
	glm::vec3 forward() { return glm::normalize(rotation); }
	bool projectionMatrixHasChanged = true;
	bool dirtyProjectionMatrix = true;
protected:
	Camera(glm::mat4 _projectionMatrix) : projectionMatrix(_projectionMatrix) {}
	glm::mat4 projectionMatrix;
};