#pragma once
#include "BindingPass.h"

class FullScreenPass : public BindingPass
{
public:
	FullScreenPass(FlatGraphics& graphics, std::string name);
	// BindingPass��(��) ���� ��ӵ�
	void Execute(FlatGraphics& graphics) const override;
};

