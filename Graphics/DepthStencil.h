#pragma once

#include "IBindable.h"
#include "BufferResource.h"

#include "DirectXHeaders.h"

class FlatGraphics;
class RenderTarget;

class DepthStencil : public IBindable, public BufferResource
{
	friend RenderTarget;

public:
	enum class Usage
	{
		DEPTHSTENCIL,
		SHADOWDEPTH
	};

	// BufferResource을(를) 통해 상속됨
	void BindBuffer(FlatGraphics& graphics) override;
	void BindBuffer(FlatGraphics& graphics, BufferResource* renderTarget) override;
	void BindBuffer(FlatGraphics& graphics, RenderTarget* renderTarget);
	void Clear(FlatGraphics& graphics) override;

	virtual void OnResize(FlatGraphics& graphics);

protected:
	DepthStencil(FlatGraphics& graphics, unsigned width, unsigned height, bool canBindShaderInput, Usage usage);

protected:
	winrt::com_ptr<ID3D11DepthStencilView> m_depthStencilView = nullptr;
	Usage m_usage = {};
};

