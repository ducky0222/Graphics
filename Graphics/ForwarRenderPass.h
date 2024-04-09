#pragma once

#include "RenderQueuePass.h"
#include "StencilState.h"

class FlatGraphics;

class ForwarRenderPass : public RenderQueuePass
{
public:
	ForwarRenderPass(FlatGraphics& graphics, std::string name)
		: RenderQueuePass(std::move(name))
	{
		// 싱크로 랜더 타겟을 필요로 하고, 소스로 랜더 타겟을 만든다.

		registerSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_renderTarget));
		registerSink(DirectBufferSink<DepthStencil>::Make("depthStencil", m_depthStencil));
		
		registerSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));
		registerSource(DirectBufferSource<DepthStencil>::Make("depthStencil", m_depthStencil));

		AddBind(StencilState::Create(graphics, StencilState::Mode::OFF));
	}

	void Execute(FlatGraphics& graphics) const override
	{
		m_depthStencil->Clear(graphics);
		RenderQueuePass::Execute(graphics);
	}
};

