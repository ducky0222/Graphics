#pragma once

#include <windows.h>

#include "DirectXHeaders.h"

class FlatGraphics
{
	friend class GraphicsResource;

public:
	FlatGraphics() = default;
	~FlatGraphics() = default;
	FlatGraphics(const FlatGraphics&) = delete;
	FlatGraphics& operator=(const FlatGraphics&) = delete;

	void Initialize(HWND hWnd, int width, int height, bool useImgui);
	void Finalize();

private:
	winrt::com_ptr<ID3D11Device> m_device = nullptr;
	winrt::com_ptr<ID3D11DeviceContext> m_context = nullptr;
	winrt::com_ptr<IDXGISwapChain> m_swapChain = nullptr;
};

