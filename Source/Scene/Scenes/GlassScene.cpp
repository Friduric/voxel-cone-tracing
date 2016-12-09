#include "GlassScene.h"

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
	unsigned int lightCubeIndex = 0;
	MaterialSetting * buddhaMaterialSetting;
	MeshRenderer * buddhaRenderer;
	MeshRenderer * sphereRenderer;
}

void GlassScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
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

	// Light cube.
	Shape * lightCube = ObjLoader::loadObjFile("Assets\\Models\\sphere.obj");
	shapes.push_back(lightCube);
	for (unsigned int i = 0; i < lightCube->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(lightCube->meshes[i])));
	}
	lightCubeIndex = renderers.size() - 1;

	// Cornell box.
	renderers[0]->materialSetting = MaterialSetting::Green(); // Green wall.
	renderers[1]->materialSetting = MaterialSetting::White(); // Floor.
	renderers[2]->materialSetting = MaterialSetting::White(); // Roof.
	renderers[3]->materialSetting = MaterialSetting::Red(); // Red wall.
	renderers[4]->materialSetting = MaterialSetting::Blue(); // White wall.
	renderers[5]->materialSetting = MaterialSetting::White(); // Left box.
	renderers[5]->enabled = false;
	renderers[6]->materialSetting = MaterialSetting::White(); // Right box.
	renderers[6]->enabled = false;

	int buddhaIndex = renderers.size();
	Shape * buddha = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(buddha);
	for (unsigned int i = 0; i < buddha->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(buddha->meshes[i])));
	}
	buddhaRenderer = renderers[buddhaIndex];
	buddhaRenderer->transform.scale = glm::vec3(1.8f);
	buddhaRenderer->transform.rotation = glm::vec3(0, 2.4, 0);
	buddhaRenderer->transform.position = glm::vec3(0, -0.13, 0.05);// glm::vec3(0, 0.0, 0);
	buddhaRenderer->transform.updateTransformMatrix();
	buddhaRenderer->tweakable = true;
	buddhaRenderer->name = "Buddha";
	buddhaRenderer->materialSetting = MaterialSetting::White();
	buddhaMaterialSetting = buddhaRenderer->materialSetting;
	buddhaMaterialSetting->specularColor = glm::vec3(0.99, 0.61, 0.43);
	buddhaMaterialSetting->diffuseColor = buddhaMaterialSetting->specularColor;
	buddhaMaterialSetting->emissivity = 0.00f;
	buddhaMaterialSetting->transparency = 1.00f;
	buddhaMaterialSetting->refractiveIndex = 1.21f;
	buddhaMaterialSetting->specularReflectivity = 1.00f;
	buddhaMaterialSetting->diffuseReflectivity = 0.0f;
	buddhaMaterialSetting->specularDiffusion = 1.9f;

	// Light cube.
	renderers[lightCubeIndex]->materialSetting = MaterialSetting::Emissive();
	renderers[lightCubeIndex]->materialSetting->diffuseColor.r = 1.0f;
	renderers[lightCubeIndex]->materialSetting->diffuseColor.g = 1.0f;
	renderers[lightCubeIndex]->materialSetting->diffuseColor.b = 1.0f;
	renderers[lightCubeIndex]->materialSetting->emissivity = 8.0f;
	renderers[lightCubeIndex]->materialSetting->specularReflectivity = 0.0f;
	renderers[lightCubeIndex]->materialSetting->diffuseReflectivity = 0.0f;

	int backWallIndex = renderers.size();
	Shape * backWall = ObjLoader::loadObjFile("Assets\\Models\\quadn.obj");
	shapes.push_back(backWall);
	for (unsigned int i = 0; i < backWall->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(backWall->meshes[i])));
	}
	MeshRenderer * bwr = renderers[backWallIndex];
	bwr->materialSetting = MaterialSetting::White();
	bwr->transform.scale = glm::vec3(2);
	bwr->transform.position = glm::vec3(0, 0, 0.99);
	bwr->transform.rotation = glm::vec3(-1.57079632679, 0, 0);
	bwr->tweakable = true;

	// Lighting.
	PointLight p;
	pointLights.push_back(p);
	pointLights[0].color = glm::vec3(0.63f, 0.47f, 0.51f);

	renderingCamera->position = glm::vec3(0, 0, 0.925);
}


void GlassScene::update() {
	FirstPersonScene::update();

	buddhaRenderer->transform.rotation.y = Time::time;

	glm::vec3 r = glm::vec3(sinf(float(Time::time * 0.67)), sinf(float(Time::time * 0.78)), cosf(float(Time::time * 0.67)));

	renderers[lightCubeIndex]->transform.position = 0.45f * r + 0.20f * r * glm::vec3(1, 0, 1);
	renderers[lightCubeIndex]->transform.scale = glm::vec3(0.049f);
	renderers[lightCubeIndex]->transform.updateTransformMatrix();

	pointLights[0].position = renderers[lightCubeIndex]->transform.position;
	renderers[lightCubeIndex]->materialSetting->diffuseColor = pointLights[0].color;
}

GlassScene::~GlassScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}
