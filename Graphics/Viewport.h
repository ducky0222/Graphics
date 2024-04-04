#pragma once

#include "IBindable.h"

class Viewport : public IBindable
{
public:
	Viewport(FlatGraphics& graphics);
	Viewport(FlatGraphics& graphics, float width, float height);

	// IBindable��(��) ���� ��ӵ�
	void Bind(FlatGraphics& graphics) override;

private:
	D3D11_VIEWPORT m_viewport = {};
};

