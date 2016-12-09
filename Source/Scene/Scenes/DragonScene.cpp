#include "DragonScene.h"

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <gtx/rotate_vector.hpp>

#include "../../Graphic/Lighting/PointLight.h"
#include "../../Time/Time.h"
#include "../../Utility/ObjLoader.h"
#include "../../Graphic/Renderer/MeshRenderer.h"
#include "../../Graphic/Material/MaterialSetting.h"

namespace {
	constexpr float UTAH_TEAPOT_SIZE = 1.0f; // 18.0f;
	unsigned int teapotIndex = 0;
	unsigned int lightCubeIndex = 0;
}

void DragonScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
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
	teapotIndex = renderers.size();

	// Dragon.
	Shape * dragon = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(dragon);
	for (unsigned int i = 0; i < dragon->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(dragon->meshes[i])));
	}
	lightCubeIndex = renderers.size();

	// Light cube.
	Shape * lightCube = ObjLoader::loadObjFile("Assets\\Models\\sphere.obj");
	shapes.push_back(lightCube);
	for (unsigned int i = 0; i < lightCube->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(lightCube->meshes[i])));
	}

	/*
	Shape * sponza = ObjLoader::loadObjFile("Assets\\Models\\sponza.obj");
	shapes.push_back(sponza);
	for (unsigned int i = 0; i < sponza->meshes.size(); ++i) {
	renderers.push_back(new MeshRenderer(&(sponza->meshes[i])));
	}
	*/

	// Cornell box.
	renderers[0]->materialSetting = MaterialSetting::Red(); // Green wall.
	renderers[1]->materialSetting = MaterialSetting::White(); // Floor.
	renderers[2]->materialSetting = MaterialSetting::White(); // Roof.
	renderers[3]->materialSetting = MaterialSetting::Blue(); // Red wall.
	renderers[3]->tweakable = true;
	renderers[4]->materialSetting = MaterialSetting::White(); // White wall.
	renderers[5]->materialSetting = MaterialSetting::White(); // Left box.
	renderers[6]->materialSetting = MaterialSetting::White(); // Right box.
	renderers[5]->enabled = false; // Disable boxes.
	renderers[6]->enabled = false; // Disable boxes.

	// Dragon.
	renderers[teapotIndex]->materialSetting = MaterialSetting::White();
	renderers[teapotIndex]->materialSetting->specularColor = glm::vec3(1.0, 0.9, 0.15);
	renderers[teapotIndex]->materialSetting->diffuseColor = renderers[teapotIndex]->materialSetting->specularColor;
	renderers[teapotIndex]->materialSetting->emissivity = 0.0f;
	renderers[teapotIndex]->materialSetting->specularReflectivity = 1.0;
	renderers[teapotIndex]->materialSetting->diffuseReflectivity = 0.0;
	renderers[teapotIndex]->materialSetting->specularDiffusion = 4.5;
	renderers[teapotIndex]->tweakable = true;

	// Dragon pot.
	for (unsigned int i = teapotIndex; i < renderers.size(); ++i) {
		auto & r = renderers[i];
		r->transform.scale = glm::vec3(1.5f);
		r->transform.rotation = glm::vec3(0, 1.0, 0);
		r->transform.position = glm::vec3(0, -0.3, 0);
		r->transform.updateTransformMatrix();
	}

	// Light cube.
	renderers[lightCubeIndex]->materialSetting = MaterialSetting::Emissive();
	renderers[lightCubeIndex]->materialSetting->diffuseColor.r = 1.0f;
	renderers[lightCubeIndex]->materialSetting->diffuseColor.g = 1.0f;
	renderers[lightCubeIndex]->materialSetting->diffuseColor.b = 1.0f;
	renderers[lightCubeIndex]->materialSetting->emissivity = 2.0f;
	renderers[lightCubeIndex]->materialSetting->specularReflectivity = 0.0f;
	renderers[lightCubeIndex]->materialSetting->diffuseReflectivity = 0.0f;

	// Lighting.
	PointLight p;
	p.color = glm::vec3(0.75f, 0.1f, 0.1f);
	pointLights.push_back(p);
	pointLights[0].color = glm::vec3(1.4f, 0.9f, 0.35f);
	pointLights[0].color = normalize(pointLights[0].color);
}

void DragonScene::update() {
	FirstPersonScene::update();

	glm::vec3 r = glm::vec3(sinf(float(Time::time * 0.97)), sinf(float(Time::time * 0.45)), sinf(float(Time::time * 0.32)));

	// Lighting.
	renderers[lightCubeIndex]->transform.position = glm::vec3(0, 0.5, 0) + r * 0.1f;
	renderers[lightCubeIndex]->transform.position.x *= 4.5f;
	renderers[lightCubeIndex]->transform.position.z *= 4.5f;
	renderers[lightCubeIndex]->transform.rotation = r;
	renderers[lightCubeIndex]->transform.scale = glm::vec3(0.049f);
	renderers[lightCubeIndex]->transform.updateTransformMatrix();
	pointLights[0].position = renderers[lightCubeIndex]->transform.position;
	renderers[lightCubeIndex]->materialSetting->diffuseColor = pointLights[0].color;
}

DragonScene::~DragonScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}