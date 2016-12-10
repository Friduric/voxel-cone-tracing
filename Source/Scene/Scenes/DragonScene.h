#pragma once

#include <vector>

#include "../Scene.h"
#include "../Templates/FirstPersonScene.h"

class Shape;

/// <summary> A scene with a specular dragon and a ceiling lamp. </summary>
class DragonScene : public FirstPersonScene {
public:
	void update() override;
	void init(unsigned int viewportWidth, unsigned int viewportHeight) override;
	~DragonScene();
private:
	std::vector<Shape*> shapes;
};