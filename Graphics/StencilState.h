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
		DEPTHOFF,		// 깊이 판정, 깊이 쓰기 끔
		DEPTHREVERSED,	// 깊이 판정 반대로, 더 깊은 오브젝트가 그려짐
		DEPTHFIRST		// SkyBox Render
	};

	StencilState(FlatGraphics& graphics, Mode mode);
	~StencilState() = default;

	static std::shared_ptr<StencilState> Create(FlatGraphics& graphics, Mode mode);
	static std::string GenerateUID(Mode mode);
	
	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	std::string GetUID() const override;

private:
	Mode m_mode = Mode::OFF;
	winrt::com_ptr<ID3D11DepthStencilState> m_stencilState = nullptr;

};

