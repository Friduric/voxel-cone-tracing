#include "MaterialStore.h"

#include <iostream>

#include "Material.h"
#include "Shader.h"

MaterialStore::MaterialStore()
{
	// Voxelization.
	AddNewMaterial("voxelization", "Voxelization\\voxelization.vert", "Voxelization\\voxelization.frag", "Voxelization\\voxelization.geom");

	// Voxelization visualization.
	AddNewMaterial("voxel_visualization", "Voxelization\\Visualization\\voxel_visualization.vert", "Voxelization\\Visualization\\voxel_visualization.frag");
	AddNewMaterial("world_position", "Voxelization\\Visualization\\world_position.vert", "Voxelization\\Visualization\\world_position.frag");

	// Cone tracing.
	AddNewMaterial("voxel_cone_tracing", "Voxel Cone Tracing\\voxel_cone_tracing.vert", "Voxel Cone Tracing\\voxel_cone_tracing.frag");
}

void MaterialStore::AddNewMaterial(
	std::string name, const char * vertexPath, const char * fragmentPath,
	const char * geometryPath, const char * tessEvalPath, const char * tessCtrlPath)
{
	// std::cout << vertexPath << ", " << fragmentPath << ", " << geometryPath << ", " << (tessEvalPath == nullptr) << ", " << (tessCtrlPath == nullptr) << std:: endl;
	using ST = Shader::ShaderType;
	Shader *v, *f, *g, *te, *tc;
	v = f = g = te = tc = nullptr;
	const std::string shaderPath = "Shaders\\";
	if (vertexPath) { v = new Shader(shaderPath + vertexPath, ST::VERTEX); }
	if (fragmentPath) { f = new Shader(shaderPath + fragmentPath, ST::FRAGMENT); }
	if (geometryPath) { g = new Shader(shaderPath + geometryPath, ST::GEOMETRY); }
	if (tessEvalPath) { te = new Shader(shaderPath + tessEvalPath, ST::TESSELATION_EVALUATION); }
	if (tessCtrlPath) { tc = new Shader(shaderPath + tessCtrlPath, ST::TESSELATION_CONTROL); }
	materials.push_back(new Material(name, v, f, g, te, tc));
	delete v, f, g, te, tc;
}

Material * MaterialStore::findMaterialWithName(std::string name)
{
	for (unsigned int i = 0; i < materials.size(); ++i) {
		if (materials[i]->name == name) {
			return materials[i];
		}
	}
	std::cerr << "Couldn't find material with name " << name << std::endl;
	return nullptr;
}

Material * MaterialStore::findMaterialWithProgramID(unsigned int programID)
{
	for (unsigned int i = 0; i < materials.size(); ++i) {
		if (materials[i]->program == programID) {
			return materials[i];
		}
	}
	std::cerr << "Couldn't find material with program ID " << programID << std::endl;
	return nullptr;
}

MaterialStore& MaterialStore::getInstance()
{
	static MaterialStore instance;
	return instance;
}

MaterialStore::~MaterialStore() {
	for (unsigned int i = 0; i < materials.size(); ++i)
	{
		delete materials[i];
	}
}