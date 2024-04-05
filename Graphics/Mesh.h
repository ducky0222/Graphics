#pragma once

#include "FlatGraphics.h"
#include "IDrawable.h"
#include "Vertex.h"

class Job;

class Mesh : public IDrawable
{
	friend class Job;

public:
	struct VertexType
	{
		DirectX::SimpleMath::Vector3 Position = { 0,0,0 };
		DirectX::SimpleMath::Vector2 UV = { 0,0 };
		DirectX::SimpleMath::Vector3 Normal = { 0,0,0 };
		DirectX::SimpleMath::Vector3 Tangent = { 0,0,0 };
	};

	Mesh(FlatGraphics& graphics, const aiMesh& mesh, Material material);
	virtual ~Mesh() = default;

	void Submit(const DirectX::SimpleMath::Matrix& modelTransform, const DirectX::SimpleMath::Matrix& texTransform, const IDrawable::AlphaData& alphaData);
	void Reset();

private:
	void parseVertices(const aiMesh& mesh);
	void parseIndices(const aiMesh& mesh);

private:
	std::string m_name;
	std::vector<VertexType> m_vertices;
	std::vector<unsigned> m_indices;

};

