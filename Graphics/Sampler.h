#pragma once

#include "IBindable.h"

class Sampler : public IBindable
{
public:
	enum class Type
	{
		ANISOTROPIC,
		BILINEAR,
		POINT,
	};

	Sampler(FlatGraphics& graphics, Type type, bool reflect, unsigned slot = 0);
	Sampler(FlatGraphics& graphics, Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode, unsigned slot = 0);

	virtual ~Sampler() = default;

	static std::shared_ptr<Sampler> Create(FlatGraphics& graphics, Type type = Type::ANISOTROPIC, bool reflect = false, unsigned slot = 0);
	static std::shared_ptr<Sampler> Create(FlatGraphics& graphics, Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode, unsigned slot);

	static std::string GenerateUID(Type type, bool reflect, unsigned slot = 0);
	static std::string GenerateUID(Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode, unsigned slot = 0);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	std::string GetUID() const override;

protected:
	winrt::com_ptr<ID3D11SamplerState> m_sampler = nullptr;
	Type m_type = Type::ANISOTROPIC;
	unsigned m_slot = 0;
	bool m_isReflect = false;
	D3D11_TEXTURE_ADDRESS_MODE m_addressMode;

};

