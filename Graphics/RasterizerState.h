#pragma once

#include "IBindable.h"

class RasterizerState : public IBindable
{
public:
	RasterizerState(FlatGraphics& graphics, bool twoSided);
	RasterizerState(FlatGraphics& graphics, D3D11_CULL_MODE cullMode);
	~RasterizerState() = default;

	static std::shared_ptr<RasterizerState> Create(FlatGraphics& graphics, bool twoSided);
	static std::shared_ptr<RasterizerState> Create(FlatGraphics& graphics, D3D11_CULL_MODE cullMode);
	static std::string GenerateUID(bool twoSided);
	static std::string GenerateUID(D3D11_CULL_MODE cullMode);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	std::string GetUID() const override;

private:
	winrt::com_ptr<ID3D11RasterizerState> m_rasterizer = nullptr;
	D3D11_CULL_MODE m_cullMode = D3D11_CULL_NONE;

};

