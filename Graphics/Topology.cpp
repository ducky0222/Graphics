#include "pch.h"
#include "Topology.h"
#include "ResourceManager.h"

Topology::Topology(FlatGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type)
	: m_type(type)
{

}

std::shared_ptr<Topology> Topology::Create(FlatGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type /*= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*/)
{
	return ResourceManager::Create<Topology>(graphics, type);
}

std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + std::to_string(type);
}

void Topology::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->IASetPrimitiveTopology(m_type);
}

std::string Topology::GetUID() const
{
	return GenerateUID(m_type);
}
