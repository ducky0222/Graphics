#pragma once

#include "Pass.h"

class BufferResource;

class BufferClearPass : public Pass
{
public:
	BufferClearPass(std::string name);

	// Pass��(��) ���� ��ӵ�
	void Execute(FlatGraphics& graphics) const override;

private:
	std::shared_ptr<BufferResource> m_buffer = nullptr;
};

