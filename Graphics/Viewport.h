#pragma once

#include "IBindable.h"

class Viewport : public IBindable
{
public:
	Viewport(FlatGraphics& graphics);
	Viewport(FlatGraphics& graphics, float width, float height);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

private:
	D3D11_VIEWPORT m_viewport = {};
};

