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

}

void Model::Submit()
{

}

void Model::Reset()
{

}
