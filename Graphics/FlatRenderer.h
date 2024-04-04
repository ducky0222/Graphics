#pragma once

#include "IRenderer.h"

class FlatRenderer : public IRenderer
{
public:
	FlatRenderer() = default;
	virtual ~FlatRenderer() = default;
	FlatRenderer(const FlatRenderer&) = delete;
	FlatRenderer& operator=(const FlatRenderer&) = delete;

	// IRenderer을(를) 통해 상속됨
	void Initialize(int hWnd, unsigned width, unsigned height, bool useImGUI) override;
	void Destroy() override;
	void OnResize(unsigned width, unsigned height) override;
	void BeginRender() override;
	void Excute() override;
	void EndRender() override;
	void Submit() override;
};

