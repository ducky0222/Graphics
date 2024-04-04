#include "pch.h"
#include "FlatRenderer.h"

FLATGRAPHICS IRenderer* Exporter::GetRenderer()
{
	if (m_renderer == nullptr)
	{
		m_renderer = new FlatRenderer;
	}

	return m_renderer;
}

FLATGRAPHICS void Exporter::DeleteRenderer()
{
	delete m_renderer;
}

IRenderer* Exporter::m_renderer = nullptr;
