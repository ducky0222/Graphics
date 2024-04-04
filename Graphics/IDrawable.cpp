#include "pch.h"
#include "IDrawable.h"
#include "Topology.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

IDrawable::IDrawable(DrawableType type /*= DrawableType::NONE*/)
	: m_drawableType(type)
{

}

void IDrawable::Submit()
{
	for (const auto& technique : m_techniques)
		technique.second.Submit(*this);
}

void IDrawable::Bind(FlatGraphics& graphics) const
{
	m_topology->Bind(graphics);
	m_indices->Bind(graphics);
	m_vertices->Bind(graphics);
}

void IDrawable::LinkTechniques(RenderGraph& renderGraph)
{
	for (auto& technique : m_techniques)
		technique.second.Link(renderGraph);
}

void IDrawable::AddTechnique(Technique technique)
{
	technique.InitReference(*this);
	m_techniques.insert({ technique.GetName(), technique });
}

unsigned IDrawable::GetIndexCount() const
{
	return m_indices->GetCount();
}
