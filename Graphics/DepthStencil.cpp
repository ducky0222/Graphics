#include "DepthStencil.h"
#include "RenderTarget.h"
#include "FlatGraphics.h"

void DepthStencil::BindBuffer(FlatGraphics&)
{
}

void DepthStencil::BindBuffer(FlatGraphics&, BufferResource*)
{
}

void DepthStencil::BindBuffer(FlatGraphics& graphics, RenderTarget* renderTarget)
{

}

void DepthStencil::Clear(FlatGraphics&)
{
}

void DepthStencil::OnResize(FlatGraphics& graphics)
{

}

DepthStencil::DepthStencil(FlatGraphics& graphics, unsigned width, unsigned height, bool canBindShaderInput, Usage usage)
{

}
