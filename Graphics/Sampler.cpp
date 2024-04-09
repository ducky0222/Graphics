#include "pch.h"
#include "Sampler.h"
#include "ResourceManager.h"

Sampler::Sampler(FlatGraphics& graphics, Type type, bool reflect, unsigned slot /*= 0*/)
	: Sampler(graphics, type, reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP, slot)
{

}

Sampler::Sampler(FlatGraphics& graphics, Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode, unsigned slot /*= 0*/)
	: m_type(type), m_addressMode(addressMode), m_slot(slot)
{
	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT() };

	samplerDesc.BorderColor[0] = { 0.f };
	samplerDesc.BorderColor[1] = { 0.f };
	samplerDesc.BorderColor[2] = { 0.f };
	samplerDesc.BorderColor[3] = { 1e5f };

	samplerDesc.Filter = [type]()
		{
			switch (type)
			{
				case Sampler::Type::ANISOTROPIC:
					return D3D11_FILTER_ANISOTROPIC;
				case Sampler::Type::BILINEAR:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;
				case Sampler::Type::POINT:
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				default:
					assert(false);
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}();

		samplerDesc.AddressU = addressMode;
		samplerDesc.AddressV = addressMode;
		samplerDesc.AddressW = addressMode;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		GetDevice(graphics)->CreateSamplerState(&samplerDesc, m_sampler.put());
}

std::shared_ptr<Sampler> Sampler::Create(FlatGraphics& graphics, Type type /*= Type::ANISOTROPIC*/, bool reflect /*= false*/, unsigned slot /*= 0*/)
{
	return ResourceManager::Create<Sampler>(graphics, type, reflect, slot);
}

std::shared_ptr<Sampler> Sampler::Create(FlatGraphics& graphics, Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode, unsigned slot)
{
	return ResourceManager::Create<Sampler>(graphics, type, addressMode, slot);
}

std::string Sampler::GenerateUID(Type type, bool reflect, unsigned slot /*= 0*/)
{
	return GenerateUID(type, reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP, slot);
}

std::string Sampler::GenerateUID(Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode, unsigned slot /*= 0*/)
{
	using namespace std::string_literals;
	return typeid(Sampler).name() + "#"s + std::to_string(static_cast<int>(type)) + std::to_string(addressMode) + std::to_string(slot);
}

void Sampler::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->PSSetSamplers(m_slot, 1, m_sampler.put());
}

std::string Sampler::GetUID() const
{
	return GenerateUID(m_type, m_isReflect);
}
