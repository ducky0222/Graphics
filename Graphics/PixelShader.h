#pragma once

#include "IBindable.h"

class FlatGraphics;

class PixelShader : public IBindable
{
public:
	PixelShader(FlatGraphics& graphics, const std::string& path);
	virtual ~PixelShader() = default;

	static std::shared_ptr<PixelShader> Create(FlatGraphics& graphics, const std::string& path);
	static std::string GenerateUID(const std::string& path);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;
	std::string GetUID() const override;

protected:
	std::string m_path;
	winrt::com_ptr<ID3D11PixelShader> m_pixelShader = nullptr;

};

