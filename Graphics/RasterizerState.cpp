#include "pch.h"
#include "RasterizerState.h"
#include "ResourceManager.h"

RasterizerState::RasterizerState(FlatGraphics& graphics, bool twoSided)
	: RasterizerState(graphics, twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK)
{

}

RasterizerState::RasterizerState(FlatGraphics& graphics, D3D11_CULL_MODE cullMode)
	: m_cullMode(cullMode)
{
	D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	desc.CullMode = cullMode;

	GetDevice(graphics)->CreateRasterizerState(&desc, m_rasterizer.put());
}

std::shared_ptr<RasterizerState> RasterizerState::Create(FlatGraphics& graphics, bool twoSided)
{
	return ResourceManager::Create<RasterizerState>(graphics, twoSided);
}

std::shared_ptr<RasterizerState> RasterizerState::Create(FlatGraphics& graphics, D3D11_CULL_MODE cullMode)
{
	return ResourceManager::Create<RasterizerState>(graphics, cullMode);
}

std::string RasterizerState::GenerateUID(bool twoSided)
{
	return GenerateUID(twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK);
}

std::string RasterizerState::GenerateUID(D3D11_CULL_MODE cullMode)
{
	using namespace std::string_literals;

	return typeid(RasterizerState).name() + "#"s + std::to_string(static_cast<int>(cullMode));
}

void RasterizerState::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->RSSetState(m_rasterizer.get());
}

std::string RasterizerState::GetUID() const
{
	return GenerateUID(m_cullMode);
}
