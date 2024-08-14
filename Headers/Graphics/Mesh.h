#pragma once

#include <vector>
#include <string>

#include "Graphics/DXCommon.h"
#include "Framework/Mathematics.h"

#include <tiny_gltf.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

class Texture;

class Mesh
{
public:
	Mesh(tinygltf::Model& model, tinygltf::Primitive& primitive, glm::mat4& transform);
	Mesh(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);

	ID3D12Resource* GetVertexBuffer();
	ID3D12Resource* GetIndexBuffer();
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView();
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView();

	const unsigned int GetIndicesCount();

private:
	void UploadBuffers();

	// TinyGLTF Loading //
	void LoadAttribute(tinygltf::Model& model, tinygltf::Primitive& primitive, const std::string& attributeType);
	void LoadIndices(tinygltf::Model& model, tinygltf::Primitive& primitive);
	void ApplyNodeTransform(const glm::mat4 transform);

public:
	std::string Name;

private:
	// Vertex & Index Data //
	ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int verticesCount;
	unsigned int indicesCount;
};