#pragma once

#include "ConstantBuffer.h"
#include "IBindable.h"
#include "IDrawable.h"

class FlatGraphics;

class PixelCBufAlpha : public IBindable
{
public:
	struct AlphaData
	{
		int useAlpha = 0;
		float alpha = 0;
		float padding[2] = {};
	};

	PixelCBufAlpha(FlatGraphics& graphics, unsigned slot = 0);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;
	void InitReference(const IDrawable& parent) override;

private:
	std::shared_ptr<PixelConstantBuffer<AlphaData>> m_pixelsCB = nullptr;
	const IDrawable* m_parent = nullptr;

};

