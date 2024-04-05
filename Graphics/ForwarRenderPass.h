#pragma once

#include "RenderQueuePass.h"

class FlatGraphics;

class ForwarRenderPass : public RenderQueuePass
{
public:
	ForwarRenderPass(FlatGraphics& graphics, std::string name)
		: RenderQueuePass(std::move(name))
	{

	}
};

