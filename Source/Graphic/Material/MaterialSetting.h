#pragma once

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>

namespace {
	const char * diffuseColorName = "material.diffuseColor";
	const char * specularColorName = "material.specularColor";
	const char * emissivityName = "material.emissivity";
	const char * transparencyName = "material.transparency";
	const char * refractiveIndexName = "material.refractiveIndex";
	const char * specularReflectanceName = "material.specularReflectivity";
	const char * diffuseReflectanceName = "material.diffuseReflectivity";
	const char * specularDiffusionName = "material.specularDiffusion";
}

/// <summary> Represents a setting for a material that can be used along with voxel cone tracing GI. </summary>
struct MaterialSetting {
	glm::vec3 diffuseColor, specularColor = glm::vec3(1);
	float specularReflectivity, diffuseReflectivity, emissivity, specularDiffusion = 2.0f;
	float transparency = 0.0f, refractiveIndex = 1.4f;

	void Upload(GLuint program, bool useProgram = true) {
		if (useProgram) glUseProgram(program);

		// Vec3s.
		glUniform3fv(glGetUniformLocation(program, diffuseColorName), 1, glm::value_ptr(diffuseColor));
		glUniform3fv(glGetUniformLocation(program, specularColorName), 1, glm::value_ptr(specularColor));

		// Floats.
		glUniform1f(glGetUniformLocation(program, emissivityName), emissivity);
		glUniform1f(glGetUniformLocation(program, specularReflectanceName), specularReflectivity);
		glUniform1f(glGetUniformLocation(program, diffuseReflectanceName), diffuseReflectivity);
		glUniform1f(glGetUniformLocation(program, specularDiffusionName), specularDiffusion);
		glUniform1f(glGetUniformLocation(program, transparencyName), transparency);
		glUniform1f(glGetUniformLocation(program, refractiveIndexName), refractiveIndex);
	}

	bool IsEmissive() { return emissivity > 0.00001f; }

	// Basic constructor.
	MaterialSetting(
		glm::vec3 _diffuseColor = glm::vec3(1),
		float _emissivity = 0.0f,
		float _specularReflectivity = 0.0f,
		float _diffuseReflectivity = 1.0f
	) :
		diffuseColor(_diffuseColor),
		emissivity(_emissivity),
		specularReflectivity(_specularReflectivity),
		diffuseReflectivity(_diffuseReflectivity)
	{}

	static MaterialSetting * Default() {
		return new MaterialSetting();
	}

	static MaterialSetting * White() {
		return new MaterialSetting(
			glm::vec3(0.97f, 0.97f, 0.97f)
		);
	}

	static MaterialSetting * Cyan() {
		return new MaterialSetting(
			glm::vec3(0.30f, 0.95f, 0.93f)
		);
	}

	static MaterialSetting * Purple() {
		return new MaterialSetting(
			glm::vec3(0.97f, 0.05f, 0.93f)
		);
	}

	static MaterialSetting * Red() {
		return new MaterialSetting(
			glm::vec3(1.0f, 0.26f, 0.27f)
		);
	}

	static MaterialSetting * Green() {
		return new MaterialSetting(
			glm::vec3(0.27f, 1.0f, 0.26f)
		);
	}

	static MaterialSetting * Blue() {
		return new MaterialSetting(
			glm::vec3(0.35f, 0.38f, 1.0f)
		);
	}

	static MaterialSetting * Emissive() {
		return new MaterialSetting(
			glm::vec3(0.85f, 0.9f, 1.0f),
			1.0f
		);
	}
};