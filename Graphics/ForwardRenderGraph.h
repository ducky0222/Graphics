#pragma once

#include "RenderGraph.h"

class FlatGraphics;
class IBindable;
class Camera;
class RenderTarget;

class ForwardRenderGraph : public RenderGraph
{
public:
	ForwardRenderGraph(FlatGraphics& graphics);

	void Execute(FlatGraphics& graphics) override;

	void OnResize(FlatGraphics& graphics, unsigned width, unsigned height);
};

