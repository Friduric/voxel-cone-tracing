#include "MeshRenderer.h"

#include "../../Shape/Mesh.h"
#include "../Material/Material.h"
#include "../../Scene/Scene.h"
#include "../../Graphic/Camera/Camera.h"
#include "../../Time/Time.h"
#include "../../Graphic/Graphics.h"
#include "../../Graphic/Lighting/PointLight.h"
#include "../../Graphic/Texture2D.h"

// ... shader variable names.
namespace {
	const char * MODEL_MATRIX_NAME = "M";
}

MeshRenderer::MeshRenderer(Mesh * _mesh, MaterialSetting * _materialSetting) : materialSetting(_materialSetting)
{
	assert(_mesh != nullptr);

	mesh = _mesh;

	setupMeshRenderer();
}

void MeshRenderer::setupMeshRenderer()
{
	if (mesh->meshUploaded) { return; }

	// Initialize VBO, VAO and EBO.
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);

	// Upload to GPU.
	reuploadIndexDataToGPU();
	reuploadVertexDataToGPU();

	mesh->meshUploaded = true;
}

MeshRenderer::~MeshRenderer()
{
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteVertexArrays(1, &mesh->vao);
	if (materialSetting != nullptr) delete materialSetting;
}

void MeshRenderer::render(const GLuint program)
{
	
	glUniformMatrix4fv(glGetUniformLocation(program, MODEL_MATRIX_NAME), 1, GL_FALSE, glm::value_ptr(transform.getTransformMatrix()));
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void MeshRenderer::reuploadIndexDataToGPU()
{
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), mesh->indices.data(), mesh->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

void MeshRenderer::reuploadVertexDataToGPU()
{
	auto dataSize = sizeof(VertexData);
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertexData.size() * dataSize, mesh->vertexData.data(), mesh->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0); // Positions.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, dataSize, 0);
	glEnableVertexAttribArray(1); // Normals.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, VertexData::normal));
}
