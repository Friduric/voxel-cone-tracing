#pragma once

#include <vector>

#include <glm.hpp>

#include "../../Graphic/Camera/Controllers/FirstPersonController.h"
#include "../Scene.h"

/// <summary> First person template scene. </summary>
class FirstPersonScene : public Scene {
public:
	virtual void init(unsigned int viewportWidth, unsigned int viewportHeight) override {
		float aspect = viewportWidth / float(viewportHeight);
		renderingCamera = new PerspectiveCamera(1.22173f, aspect);
		renderingCamera->position = glm::vec3(0, 0, 1.8f);
		firstPersonController = new FirstPersonController(renderingCamera);
	}
	virtual void update() override { firstPersonController->update(); }
	~FirstPersonScene() { delete firstPersonController; }
private:
	FirstPersonController * firstPersonController;
};