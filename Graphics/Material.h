#pragma once

#include "FlatGraphics.h"
#include "Technique.h"
#include "Vertex.h"

struct aiMaterial;
struct aiMesh;

class VertexBuffer;
class IndexBuffer;
class Texture;

// Material은 aiMaterial의 데이터를 기반으로 동적으로 bindable 데이터를 만든다.
class Material
{
public:
	Material(FlatGraphics& graphics, const std::filesystem::path& path, const aiMaterial& material);
	~Material() = default;
	
	const std::string& GetName() const { return m_name; }
	std::vector<Technique>& GetTechniques() { return m_techniques; }

private:
	std::shared_ptr<Texture> loadTexture(FlatGraphics& graphics, const std::filesystem::path& path, const aiMaterial& material, aiTextureType aitype, unsigned int bindIndex);

private:
	std::vector<Technique> m_techniques;
	std::string m_name;
};

