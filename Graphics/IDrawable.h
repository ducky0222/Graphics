#pragma once

#include "FlatGraphics.h"

class IDrawable
{
public:
	IDrawable();
	virtual ~IDrawable() = default;
	IDrawable(const IDrawable&) = delete;

	virtual void Submit();
	void Bind(FlatGraphics& graphics) const;
};

