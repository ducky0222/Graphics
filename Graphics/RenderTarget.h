#pragma once

#include "IBindable.h"
#include "BufferResource.h"

#include <array>

class FlatGraphics;
class DepthStencil;

class RenderTarget : public IBindable, public BufferResource
{
public:
	// BufferResource을(를) 통해 상속됨
	void BindBuffer(FlatGraphics& graphics) override;
	void BindBuffer(FlatGraphics& graphics, BufferResource* depthStencil) override;
	void BindBuffer(FlatGraphics& graphics, DepthStencil* depthStencil);
	void Clear(FlatGraphics& graphics) override;
	void Clear(FlatGraphics& graphics, const std::array<float, 4>& color);

	virtual void Release();
	virtual void OnResize(FlatGraphics& graphics, ID3D11Texture2D* texture);
	virtual void OnResize(FlatGraphics& graphics, unsigned width, unsigned height);

	unsigned GetWidth() const { return m_width; }
	unsigned GetHeight() const { return m_height; }

protected:
	RenderTarget(FlatGraphics& graphics, ID3D11Texture2D* texture);
	RenderTarget(FlatGraphics& graphics, unsigned width, unsigned height, DXGI_FORMAT format);

private:
	void bindBuffer(FlatGraphics& graphics, ID3D11DepthStencilView* depthStencilView);

protected:
	unsigned m_width = 0;
	unsigned m_height = 0;
	winrt::com_ptr<ID3D11RenderTargetView> m_targetView = nullptr;
	DXGI_FORMAT m_format = {};
};

class ShaderInputRenderTarget : public RenderTarget
{
public:
	ShaderInputRenderTarget(FlatGraphics& graphics, unsigned width, unsigned height, unsigned slot, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM);

	// RenderTarget을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	virtual void Release();
	virtual void OnResize(FlatGraphics& graphics, unsigned width, unsigned height) override;

	ID3D11ShaderResourceView* GetSRV() { return m_srv.get(); }

private:
	unsigned m_slot = 0;
	winrt::com_ptr<ID3D11ShaderResourceView> m_srv = nullptr;

};

class OutputOnlyRenderTarget : public RenderTarget
{
	friend class FlatGraphics;

public:
	// RenderTarget을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

private:
	OutputOnlyRenderTarget(FlatGraphics& graphics, ID3D11Texture2D* texture);

};
