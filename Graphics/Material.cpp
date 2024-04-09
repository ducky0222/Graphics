#include "pch.h"
#include "Material.h"
#include "Texture.h"
#include "TransformCBuf.h"
#include "PixelCBufAlpha.h"
#include "Utils.h"
#include "PixelShader.h"

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

		auto alphaCbuf = std::make_shared<PixelCBufAlpha>(graphics, 2);
		step.AddBindable(alphaCbuf);
		
		{
			// pbr 텍스처
			if (material.GetTextureCount(aiTextureType_METALNESS) > 0u || material.GetTextureCount(aiTextureType_SHININESS) > 0u)
			{
			}
			else
			{
				// 상수 버퍼
				Dcb::RawLayout PSLayout;
				PSLayout.Add<Dcb::Bool>("useDiffuse")
					.Add<Dcb::Bool>("useNormal")
					.Add<Dcb::Bool>("useSpecular")
					.Add<Dcb::Bool>("useAlpha");

				Dcb::Buffer PSBuffer{ std::move(PSLayout) };



				auto pixelShader = PixelShader::Create(graphics, "../Resources/Shaders/Model_PS.hlsl");
				step.AddBindable(pixelShader);
			}

			renderTech.AddStep(std::move(step));
		}
		m_techniques.push_back(std::move(renderTech));
	}

}

std::shared_ptr<Texture> Material::loadTexture(FlatGraphics& graphics, const std::filesystem::path& path, const aiMaterial& material, aiTextureType aitype, unsigned int bindIndex)
{
	std::vector<std::string> findPaths =
	{
		"../Texture/",
		"/../TestTexture/"
	};

	for (auto& findPath : findPaths)
	{
		const auto rootPath = path.parent_path().string() + findPath;

		aiString texturePath;

		if (material.GetTexture(aitype, 0, &texturePath) == AI_SUCCESS)
		{
			std::filesystem::path filePath = Utils::ToWide(texturePath.C_Str());

			return Texture::Create(graphics, rootPath + filePath.filename().string(), bindIndex);
		}
	}

	return nullptr;
}
