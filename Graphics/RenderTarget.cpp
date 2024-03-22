#include "RenderTarget.h"
#include "DepthStencil.h"

void RenderTarget::BindBuffer(FlatGraphics& graphics)
{
	ID3D11DepthStencilView* const null = nullptr;
	bindBuffer(graphics, null);
}

void RenderTarget::BindBuffer(FlatGraphics& graphics, BufferResource* depthStencil)
{
	assert(dynamic_cast<DepthStencil*>(depthStencil) != nullptr);
	BindBuffer(graphics, static_cast<DepthStencil*>(depthStencil));
}

void RenderTarget::BindBuffer(FlatGraphics& graphics, DepthStencil* depthStencil)
{
	bindBuffer(graphics, depthStencil ? depthStencil->m_depthStencilView.get() : nullptr);
}

void RenderTarget::Clear(FlatGraphics& graphics)
{
	Clear(graphics, { 1,1,1,1 });
}

void RenderTarget::Clear(FlatGraphics& graphics, const std::array<float, 4>& color)
{
	GetContext(graphics)->ClearRenderTargetView(m_targetView.get(), color.data());
}

void RenderTarget::Release()
{
	m_targetView = nullptr;
}

void RenderTarget::OnResize(FlatGraphics& graphics, ID3D11Texture2D* texture)
{
	// 텍스처로부터 RTV 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	texture->GetDesc(&textureDesc);
	m_width = textureDesc.Width;
	m_height = textureDesc.Height;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GetDevice(graphics)->CreateRenderTargetView(texture, &rtvDesc, m_targetView.put());
}

void RenderTarget::OnResize(FlatGraphics& graphics, unsigned width, unsigned height)
{
	m_width = width;
	m_height = height;

	// 텍스처 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // RTV, SRV 둘 다 사용 가능함
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	winrt::com_ptr<ID3D11Texture2D> texture;
	GetDevice(graphics)->CreateTexture2D(&textureDesc, nullptr, texture.put());

	// RTV 생성
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GetDevice(graphics)->CreateRenderTargetView(texture.get(), &rtvDesc, m_targetView.put());
}

RenderTarget::RenderTarget(FlatGraphics& graphics, ID3D11Texture2D* texture)
{
	// 텍스처로부터 RTV 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	texture->GetDesc(&textureDesc);
	m_width = textureDesc.Width;
	m_height = textureDesc.Height;
	m_format = textureDesc.Format;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GetDevice(graphics)->CreateRenderTargetView(texture, &rtvDesc, m_targetView.put());
}

RenderTarget::RenderTarget(FlatGraphics& graphics, unsigned width, unsigned height, DXGI_FORMAT format)
	: m_width(width),
	m_height(height),
	m_format(format)
{
	// 텍스처 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // RTV, SRV 둘 다 사용 가능함
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	winrt::com_ptr<ID3D11Texture2D> texture;
	GetDevice(graphics)->CreateTexture2D(&textureDesc, nullptr, texture.put());

	// RTV 생성
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GetDevice(graphics)->CreateRenderTargetView(texture.get(), &rtvDesc, m_targetView.put());
}

void RenderTarget::bindBuffer(FlatGraphics& graphics, ID3D11DepthStencilView* depthStencilView)
{
	GetContext(graphics)->OMSetRenderTargets(1, m_targetView.put(), depthStencilView);

	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	GetContext(graphics)->RSSetViewports(1u, &vp);
}

ShaderInputRenderTarget::ShaderInputRenderTarget(FlatGraphics& graphics, unsigned width, unsigned height, unsigned slot, DXGI_FORMAT format /*= DXGI_FORMAT_B8G8R8A8_UNORM*/)
	:RenderTarget(graphics, width, height, format),
	m_slot(slot)
{
	winrt::com_ptr<ID3D11Resource> resource;
	m_targetView->GetResource(resource.put());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GetDevice(graphics)->CreateShaderResourceView(resource.get(), &srvDesc, m_srv.put());
}

void ShaderInputRenderTarget::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->PSSetShaderResources(m_slot, 1, m_srv.put());
}

void ShaderInputRenderTarget::Release()
{
	RenderTarget::Release();
	m_srv = nullptr;
}

void ShaderInputRenderTarget::OnResize(FlatGraphics& graphics, unsigned width, unsigned height)
{
	RenderTarget::OnResize(graphics, width, height);

	winrt::com_ptr<ID3D11Resource> resource;
	m_targetView->GetResource(resource.put());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GetDevice(graphics)->CreateShaderResourceView(resource.get(), &srvDesc, m_srv.put());
}

void OutputOnlyRenderTarget::Bind(FlatGraphics& graphics)
{
	assert("It will be used only for output" && false);
}

OutputOnlyRenderTarget::OutputOnlyRenderTarget(FlatGraphics& graphics, ID3D11Texture2D* texture)
	: RenderTarget(graphics, texture)
{

}
