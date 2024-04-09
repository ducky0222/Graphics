#pragma once
#include "BindingPass.h"

class FullScreenPass : public BindingPass
{
public:
	FullScreenPass(FlatGraphics& graphics, std::string name);
	// BindingPass을(를) 통해 상속됨
	void Execute(FlatGraphics& graphics) const override;
};

