#include "pch.h"
#include "Viewport.h"
#include "FlatGraphics.h"

Viewport::Viewport(FlatGraphics& graphics)
	: Viewport(graphics, static_cast<float>(graphics.GetWidth()), 
		static_cast<float>(graphics.GetHeight()))
{

}

Viewport::Viewport(FlatGraphics& graphics, float width, float height)
{
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
}

void Viewport::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->RSSetViewports(1, &m_viewport);
}
