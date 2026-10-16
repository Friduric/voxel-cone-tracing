#pragma once

#include <vector>
#include <string>

class Material;

/// <summary> Manages all loaded materials and shader programs. </summary>
class MaterialStore {
public:
	static MaterialStore& getInstance();
	std::vector<Material*> materials;
	Material * MaterialStore::findMaterialWithName(std::string name);
	Material * MaterialStore::findMaterialWithProgramID(unsigned int programID);
	void AddNewMaterial(
		std::string name, const char * vertexPath = nullptr, const char * fragmentPath = nullptr,
		const char * geometryPath = nullptr, const char * tessEvalPath = nullptr, const char * tessCtrlPath = nullptr);
	~MaterialStore();
private:
	MaterialStore();
	MaterialStore(MaterialStore const &) = delete;
	void operator=(MaterialStore const &) = delete;
};