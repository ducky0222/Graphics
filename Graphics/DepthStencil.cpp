#include "DepthStencil.h"
#include "RenderTarget.h"
#include "FlatGraphics.h"

static DXGI_FORMAT MapUsageTypeless(DepthStencil::Usage usage)
{
	switch (usage)
	{
	case DepthStencil::Usage::DEPTHSTENCIL:
		return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	case DepthStencil::Usage::SHADOWDEPTH:
		return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	default:
		assert(false);
		throw std::runtime_error{ "Base usage for Typeless format map in DepthStencil" };
	}
}

static DXGI_FORMAT MapUsageTyped(DepthStencil::Usage usage)
{
	switch (usage)
	{
	case DepthStencil::Usage::DEPTHSTENCIL:
		return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DepthStencil::Usage::SHADOWDEPTH:
		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	default:
		assert(false);
		throw std::runtime_error{ "Base usage for Typeless format map in DepthStencil" };
	}
}

static DXGI_FORMAT MapUsageColored(DepthStencil::Usage usage)
{
	switch (usage)
	{
	case DepthStencil::Usage::DEPTHSTENCIL:
		return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DepthStencil::Usage::SHADOWDEPTH:
		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	default:
		assert(false);
		throw std::runtime_error{ "Base usage for Typeless format map in DepthStencil" };
	}
}

void DepthStencil::BindBuffer(FlatGraphics& graphics)
{
	GetContext(graphics)->OMSetRenderTargets(0, nullptr, m_depthStencilView.get());
}

void DepthStencil::BindBuffer(FlatGraphics& graphics, BufferResource* renderTarget)
{
	assert(dynamic_cast<RenderTarget*>(renderTarget) != nullptr);
	BindBuffer(graphics, static_cast<RenderTarget*>(renderTarget));
}

void DepthStencil::BindBuffer(FlatGraphics& graphics, RenderTarget* renderTarget)
{
	renderTarget->BindBuffer(graphics, this);
}

void DepthStencil::Clear(FlatGraphics& graphics)
{
	GetContext(graphics)->ClearDepthStencilView(m_depthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DepthStencil::OnResize(FlatGraphics& graphics)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
	m_depthStencilView->GetDesc(&descView);

	winrt::com_ptr<ID3D11Resource> buffer;
	m_depthStencilView->GetResource(buffer.put());

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	static_cast<ID3D11Texture2D*>(buffer.get())->GetDesc(&depthStencilDesc);

	depthStencilDesc.Width = graphics.GetWidth();
	depthStencilDesc.Height = graphics.GetHeight();

	winrt::com_ptr<ID3D11Texture2D> depthStencil;
	HRESULT hr = (GetDevice(graphics)->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.put()));

	GetDevice(graphics)->CreateDepthStencilView(
		depthStencil.get(),
		&descView,
		m_depthStencilView.put());
}

DepthStencil::DepthStencil(FlatGraphics& graphics, unsigned width, unsigned height, bool canBindShaderInput, Usage usage)
	: m_usage(usage)
{
	// ±Ì¿Ã Ω∫≈ŸΩ« ≈ÿΩ∫√≥ ª˝º∫
	winrt::com_ptr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = MapUsageTypeless(usage);
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | (canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0);
	HRESULT hr = (GetDevice(graphics)->CreateTexture2D(&descDepth, nullptr, depthStencil.put()));

	// ±Ì¿Ã Ω∫≈ŸΩ« ∫‰ ª˝º∫
	D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
	descView.Format = MapUsageTyped(usage);
	descView.Flags = 0;
	descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descView.Texture2D.MipSlice = 0;

	hr = GetDevice(graphics)->CreateDepthStencilView(
		depthStencil.get(),
		&descView,
		m_depthStencilView.put());
}

ShaderInputDepthStencil::ShaderInputDepthStencil(FlatGraphics& graphics, unsigned slot, Usage usage /*= Usage::DEPTHSTENCIL*/)
	: ShaderInputDepthStencil(graphics, graphics.GetWidth(), graphics.GetHeight(), slot, usage)
{

}

ShaderInputDepthStencil::ShaderInputDepthStencil(FlatGraphics& graphics, unsigned width, unsigned height, unsigned slot, Usage usage /*= Usage::DEPTHSTENCIL*/)
	: DepthStencil(graphics, width, height, true, usage)
	, slot(slot)
{
	winrt::com_ptr<ID3D11Resource> resource;
	m_depthStencilView->GetResource(resource.put());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = MapUsageColored(usage);
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	HRESULT hr = GetDevice(graphics)->CreateShaderResourceView(
		resource.get(),
		&srvDesc,
		m_srv.put());
	assert(SUCCEEDED(hr));
}

void ShaderInputDepthStencil::OnResize(FlatGraphics& graphics)
{
	DepthStencil::OnResize(graphics);

	m_srv->Release();

	winrt::com_ptr<ID3D11Resource> resource;
	m_depthStencilView->GetResource(resource.put());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = MapUsageColored(m_usage);
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	HRESULT hr = GetDevice(graphics)->CreateShaderResourceView(
		resource.get(),
		&srvDesc,
		m_srv.put());
	assert(SUCCEEDED(hr));
}

void ShaderInputDepthStencil::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->PSSetShaderResources(slot, 1, m_srv.put());
}

OutputOnlyDepthStencil::OutputOnlyDepthStencil(FlatGraphics& graphics)
	: OutputOnlyDepthStencil(graphics, graphics.GetWidth(), graphics.GetHeight())
{

}

OutputOnlyDepthStencil::OutputOnlyDepthStencil(FlatGraphics& graphics, unsigned width, unsigned height)
	: DepthStencil(graphics, width, height, false, Usage::DEPTHSTENCIL)
{

}

void OutputOnlyDepthStencil::Bind(FlatGraphics& graphics)
{
	assert("It will be used only for output" && false);
}

