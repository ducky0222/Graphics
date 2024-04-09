#include "pch.h"
#include "PixelCBufAlpha.h"

PixelCBufAlpha::PixelCBufAlpha(FlatGraphics& graphics, unsigned slot /*= 0*/)
{
	m_pixelsCB = PixelConstantBuffer<AlphaData>::Create(graphics, slot);
}

void PixelCBufAlpha::Bind(FlatGraphics& graphics)
{
	AlphaData alphaData = {};
	alphaData.alpha = m_parent->GetAlphaData().alpha;
	alphaData.useAlpha = m_parent->GetAlphaData().used;

	m_pixelsCB->Update(graphics, alphaData);
	m_pixelsCB->Bind(graphics);
}

void PixelCBufAlpha::InitReference(const IDrawable& parent)
{
	m_parent = &parent;
}
