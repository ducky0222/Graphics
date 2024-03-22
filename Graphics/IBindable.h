#pragma once

#include "GraphicsResource.h"

class FlatGraphics;

class IBindable : public GraphicsResource
{
public:
	IBindable() = default;
	virtual ~IBindable() = default;

	virtual void Bind(FlatGraphics& graphics) abstract;
	virtual void InitReference(const class IDrawable& drawable) {}
};

