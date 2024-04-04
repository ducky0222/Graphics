#include "pch.h"
#include "ForwardRenderGraph.h"
#include "BufferClearPass.h"

ForwardRenderGraph::ForwardRenderGraph(FlatGraphics& graphics)
	: RenderGraph(graphics)
{
	{
		auto pass = std::make_unique<BufferClearPass>("clearRT");
		pass->SetSinkLinkage("buffer", "$.renderTarget");
		appendPass(std::move(pass));
	}
	
	{
		auto pass = std::make_unique<BufferClearPass>("clearDS");
		pass->SetSinkLinkage("buffer", "$.masterDepth");
		appendPass(std::move(pass));
	}
}

void ForwardRenderGraph::Execute(FlatGraphics& graphics)
{

}

void ForwardRenderGraph::OnResize(FlatGraphics& graphics, unsigned width, unsigned height)
{

}
