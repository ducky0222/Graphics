#pragma once

#include "FlatGraphics.h"
#include "Technique.h"

class Material;
struct aiMesh;
class RenderGraph;
class IndexBuffer;
class VertexBuffer;
class InputLayout;
class Topology;

enum class DrawableType
{
	MESH,
	SKINNEDMESH,
	NONE
};

class IDrawable
{
public:

	struct AlphaData
	{
		float alpha = 1.0f;
		bool used = false;
	};

	IDrawable(DrawableType type = DrawableType::NONE);
	virtual ~IDrawable() = default;
	IDrawable(const IDrawable&) = delete;

	virtual void Submit();
	void Bind(FlatGraphics& graphics) const;

	void LinkTechniques(RenderGraph& renderGraph);
	void AddTechnique(Technique technique);

	const DirectX::SimpleMath::Matrix& GetTransform() const { return m_transform; }
	void SetTransform(DirectX::SimpleMath::Matrix val) { m_transform = val; }
	
	const DirectX::SimpleMath::Matrix& GetTexTransform() const { return m_texTransform; }
	void SetTexTransform(DirectX::SimpleMath::Matrix val) { m_texTransform = val; }

	const AlphaData& GetAlphaData() const { return m_alphaData; }
	void SetAlphaData(AlphaData val) { m_alphaData = val; }

	unsigned GetIndexCount() const;

protected:
	mutable DirectX::SimpleMath::Matrix m_transform;
	mutable DirectX::SimpleMath::Matrix m_texTransform;
	mutable AlphaData m_alphaData = {};

	const DrawableType m_drawableType;
	std::shared_ptr<IndexBuffer> m_indices;
	std::shared_ptr<VertexBuffer> m_vertices;
	std::shared_ptr<Topology> m_topology;
	std::map<std::string, Technique> m_techniques;
};

