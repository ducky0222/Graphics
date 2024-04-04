#include "pch.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Surface.h"

Texture::Texture(FlatGraphics& graphics, const std::string& path, unsigned slot /*= 0*/)
	: m_path(path), m_slot(slot)
{
	// 파일 로딩
	const auto surface = Surface::FromFile(path);
	m_hasAlpha = surface.AlphaLoaded();

	// 텍스처 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = surface.GetWidth();
	textureDesc.Height = surface.GetHeight();
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	winrt::com_ptr<ID3D11Texture2D> texture;
	GetDevice(graphics)->CreateTexture2D(&textureDesc, nullptr, texture.put());

	// surface로 텍스처 초기 정보 업데이트
	GetContext(graphics)->UpdateSubresource(texture.get(), 0, nullptr, surface.GetBufferPtrConst(), surface.GetWidth()*sizeof(Surface::Color), 0);

	// SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	GetDevice(graphics)->CreateShaderResourceView(texture.get(), &srvDesc, m_textureView.put());

	// 밉맵 수준 생성
	GetContext(graphics)->GenerateMips(m_textureView.get());
}

std::shared_ptr<Texture> Texture::Create(FlatGraphics& graphics, const std::string& path, unsigned slot /*= 0*/)
{
	return ResourceManager::Create<Texture>(graphics, path, slot);
}

std::string Texture::GenerateUID(const std::string& path, unsigned slot /*= 0*/)
{
	using namespace std::string_literals;
	return typeid(Texture).name() + "#"s + path + "#"s + std::to_string(slot);
}

void Texture::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->PSSetShaderResources(m_slot, 1, m_textureView.put());
}

std::string Texture::GetUID() const
{
	return GenerateUID(m_path, m_slot);
}

unsigned Texture::calculateNumberOfMipLevels(unsigned width, unsigned height)
{
	const float xSteps = std::ceil(log2(static_cast<float>(width)));
	const float ySteps = std::ceil(log2(static_cast<float>(height)));

	return static_cast<unsigned>(std::fmax(xSteps, ySteps));
}
