#include "pch.h"
#include "BufferClearPass.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Sink.h"
#include "Source.h"

BufferClearPass::BufferClearPass(std::string name)
	: Pass(std::move(name))
{
	registerSink(DirectBufferSink<BufferResource>::Make("buffer", m_buffer));
	registerSource(DirectBufferSource<BufferResource>::Make("buffer", m_buffer));
}

void BufferClearPass::Execute(FlatGraphics& graphics) const
{
	m_buffer->Clear(graphics);
}
