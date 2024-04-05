#include "pch.h"
#include "Model.h"
#include "Material.h"

Model::Model(FlatGraphics& graphics, const std::string& pathString)
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
	unsigned importFlags = aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenBoundingBoxes;

	const aiScene* scene = importer.ReadFile(pathString, importFlags);

	if (scene == nullptr)
		assert(false);

	std::vector<Material> materials;
	materials.reserve(scene->mNumMaterials);

	for (unsigned i = 0; i < scene->mNumMaterials; i++)
		materials.emplace_back(Material(graphics, pathString, *scene->mMaterials[i]));

	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;

	// 매쉬 생성
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh& mesh = *scene->mMeshes[i];
		unsigned materialIndex = mesh.mMaterialIndex;
		Material& material = materials[mesh.mMaterialIndex];

		DirectX::SimpleMath::Vector3 meshMin = { mesh.mAABB.mMin.x, mesh.mAABB.mMin.y, mesh.mAABB.mMin.z };
		DirectX::SimpleMath::Vector3 meshMax = { mesh.mAABB.mMax.x,  mesh.mAABB.mMax.y,  mesh.mAABB.mMax.z };

		min = DirectX::SimpleMath::Vector3::Min(min, meshMin);
		max = DirectX::SimpleMath::Vector3::Min(min, meshMax);

		m_meshes.push_back(std::make_unique<Mesh>(graphics, mesh, material));
	}
}

void Model::LinkTechniques(RenderGraph& renderGraph)
{
	for (auto& mesh : m_meshes)
		mesh->LinkTechniques(renderGraph);
}

void Model::Submit()
{
	m_root->Submit(m_transform, m_texTransform, m_alphaData);
}

void Model::Reset()
{
	for (auto& mesh : m_meshes)
		mesh->Reset();
}

std::unique_ptr<Node> Model::parseNodeRecursive(int& nextId, const aiNode& node)
{
	DirectX::SimpleMath::Matrix transform = (reinterpret_cast<const DirectX::SimpleMath::Matrix*>(&node.mTransformation)->Transpose());

	std::vector<Mesh*> meshes;
	meshes.reserve(node.mNumMeshes);

	for (unsigned i = 0; i < node.mNumMeshes; i++)
	{
		const unsigned MESH_INDEX = node.mMeshes[i];
		Mesh* mesh = m_meshes[MESH_INDEX].get();

		m_meshes.emplace_back(mesh);
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(meshes), transform);

	for (unsigned i = 0; i < node.mNumChildren; i++)
		pNode->addChild(parseNodeRecursive(nextId, *node.mChildren[i]));

	return pNode;
}
