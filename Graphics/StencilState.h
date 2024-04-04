#pragma once
#include "IBindable.h"

class StencilState : public IBindable
{
public:
	enum class Mode
	{
		OFF,
		WRITE,
		MASK,
		DEPTHOFF,		// ���� ����, ���� ���� ��
		DEPTHREVERSED,	// ���� ���� �ݴ��, �� ���� ������Ʈ�� �׷���
		DEPTHFIRST		// SkyBox Render
	};

	StencilState(FlatGraphics& graphics, Mode mode);
	~StencilState() = default;

	static std::shared_ptr<StencilState> Create(FlatGraphics& graphics, Mode mode);
	static std::string GenerateUID(Mode mode);
	
	// IBindable��(��) ���� ��ӵ�
	void Bind(FlatGraphics& graphics) override;

	std::string GetUID() const override;

private:
	Mode m_mode = Mode::OFF;
	winrt::com_ptr<ID3D11DepthStencilState> m_stencilState = nullptr;

};

