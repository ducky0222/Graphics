#pragma once

#include "IBindable.h"

class Texture : public IBindable
{
public:
	Texture(FlatGraphics& graphics, const std::string& path, unsigned slot = 0);
	virtual ~Texture() = default;

	static std::shared_ptr<Texture> Create(FlatGraphics& graphics, const std::string& path, unsigned slot = 0);
	static std::string GenerateUID(const std::string& path, unsigned slot = 0);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	std::string GetUID() const override;
	
	bool HasAlpha() const { return m_hasAlpha; }

private:
	static unsigned calculateNumberOfMipLevels(unsigned width, unsigned height);

protected:
	bool m_hasAlpha = false;
	std::string m_path;
	winrt::com_ptr<ID3D11ShaderResourceView> m_textureView = nullptr;

private:
	unsigned m_slot = 0;

};

