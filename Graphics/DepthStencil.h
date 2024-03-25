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

	// BufferResource��(��) ���� ��ӵ�
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

class ShaderInputDepthStencil : public DepthStencil
{
public:
	ShaderInputDepthStencil(FlatGraphics& graphics, unsigned slot, Usage usage = Usage::DEPTHSTENCIL);
	ShaderInputDepthStencil(FlatGraphics& graphics, unsigned width, unsigned height, unsigned slot, Usage usage = Usage::DEPTHSTENCIL);

	virtual void OnResize(FlatGraphics& graphics);


	// DepthStencil��(��) ���� ��ӵ�
	void Bind(FlatGraphics& graphics) override;

private:
	unsigned slot = 0;
	winrt::com_ptr<ID3D11ShaderResourceView> m_srv = nullptr;

};

class OutputOnlyDepthStencil : public DepthStencil
{
public:
	OutputOnlyDepthStencil(FlatGraphics& graphics);
	OutputOnlyDepthStencil(FlatGraphics& graphics, unsigned width, unsigned height);

	using DepthStencil::OnResize;


	// DepthStencil��(��) ���� ��ӵ�
	void Bind(FlatGraphics& graphics) override;

};