#include "Graphics/Model.h"
#include "Graphics/Mesh.h"
#include "Graphics/DXUploadBuffer.h"

#include "Utilities/Logger.h"

Model::Model(const std::string& filePath)
{
	Name = filePath.substr(filePath.find_last_of('\\') + 1);

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string error;
	std::string warning;

	bool result = loader.LoadASCIIFromFile(&model, &error, &warning, filePath);
	if (!warning.empty())
	{
		LOG(Log::MessageType::Debug, warning);
	}

	if (!error.empty())
	{
		LOG(Log::MessageType::Error, error);
	}

	if (!result)
	{
		assert(false && "Failed to parse model.");
	}

	TraverseRootNodes(model);
}

Model::Model(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount)
{
	Mesh* mesh = new Mesh(vertices, vertexCount, indices, indexCount);
	meshes.push_back(mesh);
}

Mesh* Model::GetMesh(int index)
{
	return meshes[index];
}

const std::vector<Mesh*>& Model::GetMeshes()
{
	return meshes;
}

unsigned int Model::GetMeshCount()
{
	return meshes.size();
}

void Model::TraverseRootNodes(tinygltf::Model& model)
{
	auto scene = model.scenes[model.defaultScene];
	glm::mat4 transform;

	// Traverse the 'root' nodes from the scene
	for (int i = 0; i < scene.nodes.size(); i++)
	{
		tinygltf::Node& rootNode = model.nodes[scene.nodes[i]];

		if (rootNode.matrix.size() > 0)
		{
			std::vector<float> matrix;
			for (int j = 0; j < 16; j++)
			{
				matrix.push_back(static_cast<float>(rootNode.matrix[j]));
			}

			transform = glm::make_mat4(matrix.data());
		}
		else
		{
			transform = GetTransformFromNode(rootNode);
		}

		if (rootNode.mesh != -1)
		{
			tinygltf::Mesh& mesh = model.meshes[rootNode.mesh];

			for (tinygltf::Primitive& primitive : mesh.primitives)
			{
				Mesh* m = new Mesh(model, primitive, transform);
				m->Name = mesh.name;
				meshes.push_back(m);
			}
		}

		// Process Child Nodes //
		for (int noteID : rootNode.children)
		{
			TraverseChildNodes(model, model.nodes[noteID], transform);
		}
	}
}

void Model::TraverseChildNodes(tinygltf::Model& model, tinygltf::Node& node, const glm::mat4& parentMatrix)
{
	glm::mat4 transform;

	// 1. Load matrix from node //
	if (node.matrix.size() > 0)
	{
		std::vector<float> matrix;
		for (int i = 0; i < 16; i++)
		{
			matrix.push_back(static_cast<float>(node.matrix[i]));
		}

		transform = glm::make_mat4(matrix.data());
	}
	else
	{
		// 1b. incase matrix data doesn't exist,
		// its assumed transform data is either Identity or 
		// stored as vectors Position, Rotation, Scale )
		transform = GetTransformFromNode(node);
	}

	glm::mat4 childNodeTransform = parentMatrix * transform;

	// 2. Apply to meshes in note //
	if (node.mesh != -1)
	{
		tinygltf::Mesh& mesh = model.meshes[node.mesh];

		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			Mesh* m = new Mesh(model, primitive, childNodeTransform);
			m->Name = mesh.name;
			meshes.push_back(m);
		}
	}

	// 3. Loop for children // 
	for (int noteID : node.children)
	{
		TraverseChildNodes(model, model.nodes[noteID], childNodeTransform);
	}
}

glm::mat4 Model::GetTransformFromNode(tinygltf::Node& node)
{
	::Transform transform;

	// The size of any type of transformation data defaults to 0.
	// When a vector isn't 0, it means it contains data
	if (node.translation.size() > 0)
	{
		transform.Position.x = node.translation[0];
		transform.Position.y = node.translation[1];
		transform.Position.z = node.translation[2];
	}

	if (node.rotation.size() > 0)
	{
		glm::quat rotation;
		rotation.x = node.rotation[0];
		rotation.y = node.rotation[1];
		rotation.z = node.rotation[2];
		rotation.w = node.rotation[3];

		glm::vec3 euler = glm::eulerAngles(rotation) * 180.0f / 3.14159265f;
		transform.Rotation = euler;
	}

	if (node.scale.size() > 0)
	{
		transform.Scale.x = node.scale[0];
		transform.Scale.y = node.scale[1];
		transform.Scale.z = node.scale[2];
	}

	return transform.GetModelMatrix();
}