#pragma once

#include <string>
#include <vector>
#include <tiny_gltf.h>

#include "Graphics/Transform.h"
#include "Graphics/DXCommon.h"

class Mesh;
struct Vertex;

class Model
{
	// Model through TinyglTF, so glTF files are expected
	Model(const std::string& filePath);

	// Model through pre-loaded/generated buffer
	Model(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);

	Mesh* GetMesh(int index);
	const std::vector<Mesh*>& GetMeshes();
	unsigned int GetMeshCount();

private:
	void TraverseRootNodes(tinygltf::Model& model);
	void TraverseChildNodes(tinygltf::Model& model, tinygltf::Node& node, const glm::mat4& parentMatrix);

	glm::mat4 GetTransformFromNode(tinygltf::Node& node);

public:
	Transform Transform;
	std::string Name;

private:
	std::vector<Mesh*> meshes;
};