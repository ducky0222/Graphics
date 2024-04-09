#include "pch.h"
#include "OffScreenMappingPass.h"
#include "RenderTarget.h"
#include "Sink.h"
#include "Source.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "FlatGraphics.h"

OffScreenMappingPass::OffScreenMappingPass(FlatGraphics& graphics, std::string name)
	: FullScreenPass(graphics, std::move(name))
{
	AddBindSink<RenderTarget>("renderTargetSRV");
	registerSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_renderTarget));
	registerSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));

	AddBind(PixelShader::Create(graphics, "../Resources/Shaders/Fullscreen_PS.hlsl"));
	AddBind(Sampler::Create(graphics, Sampler::Type::POINT, true));
}

void OffScreenMappingPass::Execute(FlatGraphics& graphics) const
{
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;
	graphics.GetContext()->OMSetRenderTargets(1, &rtv, dsv);
	FullScreenPass::Execute(graphics);
}
