#include "pch.h"
#include "Material.h"
#include "Texture.h"
#include "TransformCBuf.h"

Material::Material(FlatGraphics& graphics, const std::filesystem::path& path, const aiMaterial& material)
{
	aiString name;
	material.Get(AI_MATKEY_NAME, name);
	m_name = name.C_Str();

	int textureCounts[22] = {};
	for (int i = 0; i < 22; i++)
		textureCounts[i] = material.GetTextureCount(static_cast<aiTextureType>(i));

	{
		Technique renderTech{ "RenderTechnique" };

		Step step{ "renderPass" };

		auto transformCbuf = std::make_shared<TransformCBuf>(graphics, 0);
		step.AddBindable(transformCbuf);
	}

}

std::shared_ptr<Texture> Material::loadTexture(FlatGraphics& graphics, const std::filesystem::path& path, const aiMaterial& material, aiTextureType aitype, unsigned int bindIndex)
{
	return nullptr;
}
