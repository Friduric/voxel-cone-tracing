#pragma once

#include <vector>

#include "../Scene.h"
#include "../Templates/FirstPersonScene.h"

class Shape;

/// <summary> A scene with multiple different objects. </summary>
class MultipleObjectsScene : public FirstPersonScene {
public:
	void update() override;
	void init(unsigned int viewportWidth, unsigned int viewportHeight) override;
	~MultipleObjectsScene();
private:
	std::vector<Shape*> shapes;
};