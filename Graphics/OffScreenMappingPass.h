#pragma once

#include "FullScreenPass.h"

class OffScreenMappingPass : public FullScreenPass
{
	OffScreenMappingPass(FlatGraphics& graphics, std::string name);
	// BindingPass��(��) ���� ��ӵ�
	void Execute(FlatGraphics& graphics) const override;
};

