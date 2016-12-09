#include "TransparencyScene.h"

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <gtx/rotate_vector.hpp>

#include "../../Graphic/Lighting/PointLight.h"
#include "../../Graphic/Camera/Camera.h"
#include "../../Graphic/Camera/PerspectiveCamera.h"
#include "../../Time/Time.h"
#include "../../Utility/ObjLoader.h"
#include "../../Graphic/Renderer/MeshRenderer.h"
#include "../../Graphic/Material/MaterialSetting.h"
#include "../../Application.h"

namespace { MeshRenderer * lampRenderer; }

void TransparencyScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets\\Models\\cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) renderers.push_back(new MeshRenderer(&(cornell->meshes[i])));
	for (auto & r : renderers) {
		r->transform.position -= glm::vec3(0.00f, 0.0f, 0);
		r->transform.scale = glm::vec3(0.995f);
		r->transform.updateTransformMatrix();
	}

	renderers[0]->materialSetting = MaterialSetting::Red(); // Green wall.
	renderers[1]->materialSetting = MaterialSetting::White(); // Floor.
	renderers[2]->materialSetting = MaterialSetting::White(); // Roof.
	renderers[3]->materialSetting = MaterialSetting::Blue(); // Red wall.
	renderers[4]->materialSetting = MaterialSetting::White(); // White wall.
	renderers[5]->materialSetting = MaterialSetting::White(); // Left box.
	renderers[6]->materialSetting = MaterialSetting::White(); // Right box.
	renderers[5]->enabled = false; // Disable boxes.
	renderers[6]->enabled = false; // Disable boxes.

	// Dragon.
	int dragonIndex = renderers.size();
	Shape * dragon = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(dragon);
	for (unsigned int i = 0; i < dragon->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(dragon->meshes[i])));
	}
	auto * dragonRenderer = renderers[dragonIndex];
	dragonRenderer->transform.scale = glm::vec3(1.31f);
	dragonRenderer->transform.rotation = glm::vec3(0, 2.4, 0);
	dragonRenderer->transform.position = glm::vec3(0, -0.13, 0);// glm::vec3(0, 0.0, 0);
	dragonRenderer->transform.updateTransformMatrix();
	dragonRenderer->tweakable = true;
	dragonRenderer->name = "Dragon";
	dragonRenderer->materialSetting = MaterialSetting::White();

	auto * dragonMaterialSetting = dragonRenderer->materialSetting;
	dragonMaterialSetting->specularColor = glm::vec3(0.89, 1, 0.89);
	dragonMaterialSetting->diffuseColor = dragonMaterialSetting->specularColor;
	dragonMaterialSetting->emissivity = 0.00f;
	dragonMaterialSetting->transparency = 0.95f;
	dragonMaterialSetting->refractiveIndex = 1.18f;
	dragonMaterialSetting->specularReflectivity = 1.00f;
	dragonMaterialSetting->diffuseReflectivity = 0.0f;
	dragonMaterialSetting->specularDiffusion = 2.3f;

	// Light.
	Shape * light = ObjLoader::loadObjFile("Assets\\Models\\quad.obj");
	shapes.push_back(light);
	lampRenderer = new MeshRenderer(&(light->meshes[0]));
	renderers.push_back(lampRenderer);

	lampRenderer->materialSetting = MaterialSetting::Emissive();
	lampRenderer->materialSetting->diffuseColor.r = 1.f;
	lampRenderer->materialSetting->diffuseColor.g = 1.f;
	lampRenderer->materialSetting->diffuseColor.b = 1.f;
	lampRenderer->materialSetting->emissivity = 1.0f;
	lampRenderer->materialSetting->specularReflectivity = 0.0f;
	lampRenderer->materialSetting->diffuseReflectivity = 1.0f;

	lampRenderer->transform.position = glm::vec3(0, 0.975, 0);
	lampRenderer->transform.rotation = glm::vec3(-3.1414 * 0.5, 3.1414 * 0.5, 0);
	lampRenderer->transform.scale = glm::vec3(0.14f, 0.34f, 1.0f);
	lampRenderer->transform.updateTransformMatrix();
	lampRenderer->name = "Ceiling lamp";

	// Point light.
	PointLight p;
	p.color = glm::vec3(0.5);
	p.position = lampRenderer->transform.position - glm::vec3(0, 0.2, 0);
	pointLights.push_back(p);
}

void TransparencyScene::update() {
	FirstPersonScene::update();

	glm::vec3 col = pointLights[0].color;
	float m = col.r;
	m = glm::max(m, col.b);
	m = glm::max(m, col.g);

	lampRenderer->materialSetting->diffuseColor = col /= m;
	lampRenderer->materialSetting->emissivity = 2 * m;
}

TransparencyScene::~TransparencyScene() {
	for (auto * s : shapes) delete s;
	for (auto * r : renderers) delete r;
}