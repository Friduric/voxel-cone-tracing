#pragma once

#include <vector>

#include "../Scene.h"
#include "../Templates/FirstPersonScene.h"

class Shape;

/// <summary> First transparency test scene. </summary>
class TransparencyScene : public FirstPersonScene {
public:
	void update() override;
	void init(unsigned int viewportWidth, unsigned int viewportHeight) override;
	~TransparencyScene();
private:
	std::vector<Shape*> shapes;
};