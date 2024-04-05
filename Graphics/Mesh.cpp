#include "pch.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "TransformCBuf.h"

static DirectX::SimpleMath::Matrix convertMatrix(const aiMatrix4x4& aiMatrix)
{
	DirectX::SimpleMath::Matrix result;

	for (int i = 0; i < 4; ++i)
	{
		result.m[i][0] = aiMatrix[i][0];
		result.m[i][1] = aiMatrix[i][1];
		result.m[i][2] = aiMatrix[i][2];
		result.m[i][3] = aiMatrix[i][3];
	}

	return result;
}

Mesh::Mesh(FlatGraphics& graphics, const aiMesh& mesh, Material material)
	: IDrawable(DrawableType::MESH)
	, m_name(mesh.mName.C_Str())
{
	assert(!mesh.HasBones());

	parseVertices(mesh);
	parseIndices(mesh);

	Dvtx::VertexLayout vertexLayout;
	vertexLayout.Append(Dvtx::VertexLayout::Position3D)
		.Append(Dvtx::VertexLayout::Texture2D)
		.Append(Dvtx::VertexLayout::Normal)
		.Append(Dvtx::VertexLayout::Tangent);

	Dvtx::VertexBuffer vertices{ vertexLayout };

	for (auto& vertex : m_vertices)
		vertices.EmplaceBack(vertex.Position, vertex.UV, vertex.Normal, vertex.Tangent);

	m_pVertices = VertexBuffer::Create(graphics, m_name + material.GetName(), vertices);
	m_pIndices = IndexBuffer::Create(graphics, m_name + material.GetName(), m_indices);
	m_topology = Topology::Create(graphics);

	auto vertexShader = VertexShader::Create(graphics, "../Resources/Shaders/Model_VS.hlsl");
	auto inputLayout = InputLayout::Create(graphics, vertexLayout, vertexShader->GetByteCode());

	for (Technique& tech : material.GetTechniques())
	{
		if (tech.GetName() == "RenderTechnique")
		{
			for (Step& step : tech.GetSteps())
			{
				step.AddBindable(vertexShader);
				step.AddBindable(inputLayout);
				step.AddBindable(std::make_shared<TransformCBuf>(graphics, 0));
			}
		}

		AddTechnique(tech);
	}
}

void Mesh::Submit(const DirectX::SimpleMath::Matrix& modelTransform, const DirectX::SimpleMath::Matrix& texTransform, const IDrawable::AlphaData& alphaData)
{
	IDrawable::Submit();
}

void Mesh::Reset()
{

}

void Mesh::parseVertices(const aiMesh& mesh)
{
	for (unsigned i = 0; i < mesh.mNumVertices; i++)
	{
		VertexType vertex;

		if (mesh.HasPositions())
		{
			vertex.Position.x = mesh.mVertices[i].x;
			vertex.Position.y = mesh.mVertices[i].y;
			vertex.Position.z = mesh.mVertices[i].z;
		}

		if (mesh.HasNormals())
		{
			vertex.Normal.x = mesh.mNormals[i].x;
			vertex.Normal.y = mesh.mNormals[i].y;
			vertex.Normal.z = mesh.mNormals[i].z;
		}

		if (mesh.HasTangentsAndBitangents())
		{
			vertex.Tangent.x = mesh.mTangents[i].x;
			vertex.Tangent.y = mesh.mTangents[i].y;
			vertex.Tangent.z = mesh.mTangents[i].z;
		}

		if (mesh.HasTextureCoords(0))
		{
			vertex.UV.x = static_cast<float>(mesh.mTextureCoords[0][i].x);
			vertex.UV.y = static_cast<float>(mesh.mTextureCoords[0][i].y);
		}

		m_vertices.emplace_back(vertex);
	}
}

void Mesh::parseIndices(const aiMesh& mesh)
{
	m_indices.reserve(mesh.mNumFaces * 3);

	for (unsigned i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);

		m_indices.emplace_back(face.mIndices[0]);
		m_indices.emplace_back(face.mIndices[1]);
		m_indices.emplace_back(face.mIndices[2]);
	}
}
