#include "pch.h"
#include "Node.h"
#include "Mesh.h"

Node::Node(int id, const std::string& name, std::vector<Mesh*> meshRef, const DirectX::SimpleMath::Matrix& transform)
	: m_id(id)
	, m_bindPoseTransform(transform)
	, m_localTransform(transform)
	, m_parentTransform(DirectX::SimpleMath::Matrix::Identity)
	, m_meshRef(std::move(meshRef))
	, m_name(name)
{

}

void Node::Submit(const DirectX::SimpleMath::Matrix& modelTransform, const DirectX::SimpleMath::Matrix& texTranform, const IDrawable::AlphaData& alphaData)
{
	submitNodeRecursive(modelTransform);
	submitMeshRecursive(texTranform, alphaData);
}

void Node::submitNodeRecursive(const DirectX::SimpleMath::Matrix& parentTransform)
{
	m_parentTransform = parentTransform;

	for (auto& child : m_children)
		child->submitNodeRecursive(GetFinalTransform());
}

void Node::submitMeshRecursive(const DirectX::SimpleMath::Matrix& texTransform, const IDrawable::AlphaData& alphaData)
{
	for (auto& mesh : m_meshRef)
		mesh->Submit(GetFinalTransform(), texTransform, alphaData);

	for (auto& child : m_children)
		child->submitMeshRecursive(texTransform, alphaData);
}

void Node::addChild(std::unique_ptr<Node> child)
{
	assert(child != nullptr);

	m_children.emplace_back(std::move(child));
}
