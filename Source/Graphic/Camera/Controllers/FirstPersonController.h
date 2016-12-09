#pragma once

#include <iostream> // TODO: Remove.

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <gtx/rotate_vector.hpp>

#include "../../Camera/Camera.h"
#include "../../../Time/Time.h"
#include "../../Camera\PerspectiveCamera.h"
#include "../../../Application.h"

/// <summary> A first person controller that can be attached to a camera. </summary>
class FirstPersonController {
public:
	const float CAMERA_SPEED = 1.4f;
	const float CAMERA_ROTATION_SPEED = 0.003f;
	const float CAMERA_POSITION_INTERPOLATION_SPEED = 8.0f;
	const float CAMERA_ROTATION_INTERPOLATION_SPEED = 8.0f;

	Camera * renderingCamera;
	Camera * targetCamera; // Dummy camera used for interpolation.

	FirstPersonController(Camera * camera) {
		targetCamera = new PerspectiveCamera();
		renderingCamera = camera;

	}

	FirstPersonController() { delete targetCamera; }

	void update() {
		auto & app = Application::getInstance();
		if (app.currentInputState == Application::InputState::TWEAK_BAR) return;

		if (firstUpdate) {
			targetCamera->rotation = renderingCamera->rotation;
			targetCamera->position = renderingCamera->position;
			firstUpdate = false;
		}

		int xwidth, yheight;
		double xpos, ypos;
		double xmid, ymid;

		GLFWwindow * window = Application::getInstance().currentWindow;

		glfwGetWindowSize(window, &xwidth, &yheight);
		glfwGetCursorPos(window, &xpos, &ypos);

		xmid = xwidth / 2.0;
		ymid = yheight / 2.0;

		// ----------
		// Rotation.
		// ----------
		float xDelta = float(xmid - xpos);
		float yDelta = float(ymid - ypos);

		float xRot = static_cast<float>(CAMERA_ROTATION_SPEED * xDelta);
		float yRot = static_cast<float>(CAMERA_ROTATION_SPEED * yDelta);

		// X rotation.

		targetCamera->rotation = glm::rotateY(targetCamera->rotation, xRot);

		// Y rotation.
		glm::vec3 newDirection = glm::rotate(targetCamera->rotation, yRot, targetCamera->right());
		float a = glm::dot(newDirection, glm::vec3(0, 1, 0));
		if (abs(a) < 0.99)
			targetCamera->rotation = newDirection;


		// ----------
		// Position.
		// ----------
		// Move forward.
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			targetCamera->position += targetCamera->forward() * (float)Time::deltaTime * CAMERA_SPEED;
		}
		// Move backward.
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			targetCamera->position -= targetCamera->forward() * (float)Time::deltaTime * CAMERA_SPEED;
		}
		// Strafe right.
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			targetCamera->position += targetCamera->right() * (float)Time::deltaTime * CAMERA_SPEED;
		}
		// Strafe left.
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			targetCamera->position -= targetCamera->right() * (float)Time::deltaTime * CAMERA_SPEED;
		}

		// Interpolate between target and current camera.
		auto * camera = renderingCamera;
		camera->rotation = mix(camera->rotation, targetCamera->rotation, glm::clamp(Time::deltaTime * CAMERA_ROTATION_INTERPOLATION_SPEED, 0.0, 1.0));
		camera->position = mix(camera->position, targetCamera->position, glm::clamp(Time::deltaTime * CAMERA_POSITION_INTERPOLATION_SPEED, 0.0, 1.0));

		// Reset mouse position for next update iteration.
		glfwSetCursorPos(window, xwidth / 2, yheight / 2);

		// Update view (camera) matrix.
		camera->updateViewMatrix();
	}
private:
	bool firstUpdate = true;
};
