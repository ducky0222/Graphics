#pragma once

#include "FullScreenPass.h"

class OffScreenMappingPass : public FullScreenPass
{
	OffScreenMappingPass(FlatGraphics& graphics, std::string name);
	// BindingPass을(를) 통해 상속됨
	void Execute(FlatGraphics& graphics) const override;
};

