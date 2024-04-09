#include "pch.h"
#include "ForwardRenderGraph.h"
#include "BufferClearPass.h"
#include "ForwarRenderPass.h"

ForwardRenderGraph::ForwardRenderGraph(FlatGraphics& graphics)
	: RenderGraph(graphics)
{
	{
		auto pass = std::make_unique<BufferClearPass>("clearRT");
		pass->SetSinkLinkage("buffer", "$.backBuffer");
		appendPass(std::move(pass));
	}
	
	{
		auto pass = std::make_unique<BufferClearPass>("clearDS");
		pass->SetSinkLinkage("buffer", "$.masterDepth");
		appendPass(std::move(pass));
	}

	{
		auto pass = std::make_unique<ForwarRenderPass>(graphics, "renderPass");
		pass->SetSinkLinkage("renderTarget", "clearRT.buffer");
		pass->SetSinkLinkage("depthStencil", "clearDS.buffer");
		//pass->SetSinkLinkage("renderTarget", "$.backBuffer");

		appendPass(std::move(pass));
	}

	setSinkTarget("backbuffer", "renderPass.renderTarget");

	finalize();
}

void ForwardRenderGraph::Execute(FlatGraphics& graphics)
{
	assert(m_isFinalized);

	for (auto& pass : m_passes)
		pass->Execute(graphics);
}

void ForwardRenderGraph::OnResize(FlatGraphics& graphics, unsigned width, unsigned height)
{
	assert(m_isFinalized);

	for (auto& pass : m_passes)
		pass->OnResize(graphics, width, height);
}
