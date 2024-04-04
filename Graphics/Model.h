#pragma once

#include "FlatGraphics.h"
#include "Mesh.h"

class RenderGraph;
class Job;

class Model
{
public:
	Model(FlatGraphics& graphics, const std::string& pathString);
	~Model() = default;

	void LinkTechniques(RenderGraph& renderGraph);
	void Submit();
	void Reset();

	const DirectX::SimpleMath::Matrix& GetTransform() const { return m_transform; }
	void SetTransform(DirectX::SimpleMath::Matrix val) { m_transform = val; }

	const DirectX::SimpleMath::Matrix& GetTexTransform() const { return m_texTransform; }
	void SetTexTransform(DirectX::SimpleMath::Matrix val) { m_texTransform = val; }

	const IDrawable::AlphaData& GetAlphaData() const { return m_alphaData; }
	void SetAlphaData(IDrawable::AlphaData val) { m_alphaData = val; }

	const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_meshes; }
private:
	mutable DirectX::SimpleMath::Matrix m_transform;
	mutable DirectX::SimpleMath::Matrix m_texTransform;
	mutable IDrawable::AlphaData m_alphaData = {};

	std::vector<std::unique_ptr<Mesh>> m_meshes;
};

