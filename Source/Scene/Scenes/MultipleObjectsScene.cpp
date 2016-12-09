#include "MultipleObjectsScene.h"

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

namespace { MaterialSetting * objectMaterialSetting; }

void MultipleObjectsScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets\\Models\\cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(cornell->meshes[i])));
	}
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

	// Susanne.
	int objectIndex = renderers.size();
	Shape * object = ObjLoader::loadObjFile("Assets\\Models\\susanne.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(object->meshes[i])));
	}

	MeshRenderer * objectRenderer = renderers[objectIndex];
	objectRenderer->materialSetting = MaterialSetting::White();
	objectMaterialSetting = objectRenderer->materialSetting;
	objectMaterialSetting->specularColor = glm::vec3(0.2, 0.8, 1.0);
	objectMaterialSetting->diffuseColor = objectMaterialSetting->specularColor;
	objectMaterialSetting->emissivity = 0.00f;
	objectMaterialSetting->specularReflectivity = 1.0f;
	objectMaterialSetting->diffuseReflectivity = 0.0f;
	objectMaterialSetting->specularDiffusion = 3.2f;
	objectMaterialSetting->transparency = 0.9f;
	objectRenderer->tweakable = true;
	objectRenderer->transform.scale = glm::vec3(0.23f);
	objectRenderer->transform.rotation = glm::vec3(0.00, 0.30, 0.00);
	objectRenderer->transform.position = glm::vec3(0.07, -0.49, 0.36);
	objectRenderer->transform.updateTransformMatrix();

	// Dragon.
	objectIndex = renderers.size();
	object = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(object->meshes[i])));
	}

	objectRenderer = renderers[objectIndex];
	objectRenderer->materialSetting = MaterialSetting::White();
	objectMaterialSetting = objectRenderer->materialSetting;
	objectMaterialSetting->specularColor = glm::vec3(1.0, 0.8, 0.6);
	objectMaterialSetting->diffuseColor = objectMaterialSetting->specularColor;
	objectMaterialSetting->emissivity = 0.00f;
	objectMaterialSetting->specularReflectivity = 1.0f;
	objectMaterialSetting->diffuseReflectivity = 0.0f;
	objectMaterialSetting->specularDiffusion = 2.2f;
	objectRenderer->tweakable = true;
	objectRenderer->transform.scale = glm::vec3(1.3f);
	objectRenderer->transform.rotation = glm::vec3(0, 2.1, 0);
	objectRenderer->transform.position = glm::vec3(-0.28, -0.52, 0.00);
	objectRenderer->transform.updateTransformMatrix();

	// Bunny.
	objectIndex = renderers.size();
	object = ObjLoader::loadObjFile("Assets\\Models\\bunny.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(object->meshes[i])));
	}

	objectRenderer = renderers[objectIndex];
	objectRenderer->materialSetting = MaterialSetting::White();
	objectMaterialSetting = objectRenderer->materialSetting;
	objectMaterialSetting->specularColor = glm::vec3(0.7, 0.8, 0.7);
	objectMaterialSetting->diffuseColor = objectMaterialSetting->specularColor;
	objectMaterialSetting->emissivity = 0.00f;
	objectMaterialSetting->specularReflectivity = 0.6f;
	objectMaterialSetting->diffuseReflectivity = 0.4f;
	objectMaterialSetting->specularDiffusion = 9.4f;
	objectRenderer->tweakable = true;
	objectRenderer->transform.scale = glm::vec3(0.31f);
	objectRenderer->transform.rotation = glm::vec3(0, 0.4, 0);
	objectRenderer->transform.position = glm::vec3(0.44, -0.52, 0);
	objectRenderer->transform.updateTransformMatrix();

	// Light.
	int lightIndex = renderers.size();
	Shape * light = ObjLoader::loadObjFile("Assets\\Models\\quad.obj");
	shapes.push_back(light);
	MeshRenderer * lamp = new MeshRenderer(&(light->meshes[0]));
	renderers.push_back(lamp);

	lamp->materialSetting = MaterialSetting::Emissive();
	lamp->materialSetting->diffuseColor.r = 2.f;
	lamp->materialSetting->diffuseColor.g = 2.f;
	lamp->materialSetting->diffuseColor.b = 2.f;
	lamp->materialSetting->emissivity = 0.5f;
	lamp->materialSetting->specularReflectivity = 0.0f;
	lamp->materialSetting->diffuseReflectivity = 1.0f;

	lamp->transform.position = glm::vec3(0, 0.975, 0);
	lamp->transform.rotation = glm::vec3(-3.1414 * 0.5, 3.1414 * 0.5, 0);
	lamp->transform.scale = glm::vec3(0.14f, 0.34f, 1.0f);
	lamp->transform.updateTransformMatrix();

	// Point light.
	PointLight p;
	p.color = glm::vec3(1.0f);
	p.position = lamp->transform.position - glm::vec3(0, 0.38, 0);
	pointLights.push_back(p);
}

void MultipleObjectsScene::update() { FirstPersonScene::update(); }

MultipleObjectsScene::~MultipleObjectsScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}