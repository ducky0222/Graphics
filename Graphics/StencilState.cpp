#include "pch.h"
#include "StencilState.h"
#include "ResourceManager.h"

StencilState::StencilState(FlatGraphics& graphics, Mode mode)
	: m_mode(mode)
{
	D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	switch (mode)
	{
	case StencilState::Mode::OFF:
		break;
	case StencilState::Mode::WRITE:
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.StencilEnable = true;
		desc.StencilWriteMask = 0xFF;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case StencilState::Mode::MASK:
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.StencilEnable = true;
		desc.StencilReadMask = 0xFF;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	case StencilState::Mode::DEPTHOFF:
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		break;
	case StencilState::Mode::DEPTHREVERSED:
		desc.DepthFunc = D3D11_COMPARISON_GREATER;
		break;
	case StencilState::Mode::DEPTHFIRST:
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; 
		break;
	default:
		assert(false);
		break;
	}

	GetDevice(graphics)->CreateDepthStencilState(&desc, m_stencilState.put());
}

std::shared_ptr<StencilState> StencilState::Create(FlatGraphics& graphics, Mode mode)
{
	return ResourceManager::Create<StencilState>(graphics, mode);
}

std::string StencilState::GenerateUID(Mode mode)
{
	using namespace std::string_literals;
	const auto modeName = [mode]()
		{
			switch (mode)
			{
			case StencilState::Mode::OFF:
				return "off"s;
			case StencilState::Mode::WRITE:
				return "write"s;
			case StencilState::Mode::MASK:
				return "mask"s;
			case StencilState::Mode::DEPTHOFF:
				return "depth-off"s;
			case StencilState::Mode::DEPTHREVERSED:
				return "depth-reversed"s;
			case StencilState::Mode::DEPTHFIRST:
				return "depth-first"s;
			}
			return "error"s;
		};
	return typeid(StencilState).name() + "#"s + modeName();
}

void StencilState::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->OMSetDepthStencilState(m_stencilState.get(), 0xff);
}

std::string StencilState::GetUID() const
{
	return GenerateUID(m_mode);
}
