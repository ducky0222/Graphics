#pragma once

#include "FlatGraphics.h"
#include "IDrawable.h"

class Model;
class Mesh;

class Node
{
	friend class Model;
	
public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshRef, const DirectX::SimpleMath::Matrix& transform);
	~Node() = default;

	void Submit(const DirectX::SimpleMath::Matrix& modelTransform, const DirectX::SimpleMath::Matrix& texTranform, const IDrawable::AlphaData& alphaData);

	int GetId() const { return m_id; }
	const std::string& GetName() const { return m_name; }
	const DirectX::SimpleMath::Matrix& GetLocalTransform() const { return m_localTransform; }
	const DirectX::SimpleMath::Matrix& GetParentTransform() const { return m_parentTransform; }
	DirectX::SimpleMath::Matrix GetFinalTransform() const { return m_localTransform * m_parentTransform; }

private:
	void submitNodeRecursive(const DirectX::SimpleMath::Matrix& parentTransform);
	void submitMeshRecursive(const DirectX::SimpleMath::Matrix& texTransform, const IDrawable::AlphaData& alphaData);;
	void addChild(std::unique_ptr<Node> child);

private:
	int m_id = 0;
	std::string m_name;
	std::vector<std::unique_ptr<Node>> m_children;
	std::vector<Mesh*> m_meshRef;

	const DirectX::SimpleMath::Matrix m_bindPoseTransform;
	DirectX::SimpleMath::Matrix m_localTransform;
	DirectX::SimpleMath::Matrix m_parentTransform;
};

