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

	for(unsigned i=0;i<scene->mNumMaterials;i++)
		//materials.emplace_back(Material(graphics, ))
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
