#pragma once

#include <vector>

#include "../Templates/FirstPersonScene.h"

class Shape;

/// <summary> A test scene with a Cornell box and a flying light (emissive) ball. </summary>
class CornellScene : public FirstPersonScene {
public:
	void update() override;
	void init(unsigned int viewportWidth, unsigned int viewportHeight) override;
	~CornellScene();
private:
	std::vector<Shape*> shapes;
};