#pragma once

#include <vector>

#include "../Templates/FirstPersonScene.h"

class Shape;

/// <summary> A scene with a dragon and a lamp in the ceiling. </summary>
class DragonScene : public FirstPersonScene {
public:
	void update() override;
	void init(unsigned int viewportWidth, unsigned int viewportHeight) override;
	~DragonScene();
private:
	std::vector<Shape*> shapes;
};