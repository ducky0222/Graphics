#pragma once

#include "Camera.h"

class RenderTarget;
class DepthStencil;

class FlatGraphics
{
	friend class GraphicsResource;

public:
	FlatGraphics() = default;
	~FlatGraphics() = default;
	FlatGraphics(const FlatGraphics&) = delete;
	FlatGraphics& operator=(const FlatGraphics&) = delete;

	void Initialize(HWND hWnd, unsigned width, unsigned height, bool useImgui);
	void Finalize() const;

	void OnResize(unsigned width, unsigned height);

	void BeginFrame() const;
	void DrawIndexed(unsigned count);
	void EndFrame();

	/// Getter Setter
	unsigned GetWidth() const { return m_width; }
	unsigned GetHeight() const { return m_height; }
	bool GetUseImGui() const { return m_useImGui; }
	void SetUseImGui(bool val) { m_useImGui = val; }

	winrt::com_ptr<ID3D11Device> GetDevice() const { return m_device; }
	winrt::com_ptr<ID3D11DeviceContext> GetContext() const { return m_context; }

	std::shared_ptr<RenderTarget> GetRenderTarget() const { return m_renderTarget; }
	std::shared_ptr<RenderTarget> GetBackBuffer() const { return m_backBuffer; }
	std::shared_ptr<DepthStencil> GetDepthStencil() const { return m_depthStencil; }

private:
	winrt::com_ptr<ID3D11Device> m_device = nullptr;
	winrt::com_ptr<ID3D11DeviceContext> m_context = nullptr;
	winrt::com_ptr<IDXGISwapChain> m_swapChain = nullptr;

	// TODO: deferred rendering 사용시 mrt로 수정
	std::shared_ptr<RenderTarget> m_renderTarget = nullptr;
	std::shared_ptr<RenderTarget> m_backBuffer = nullptr;
	std::shared_ptr<DepthStencil> m_depthStencil = nullptr;

	unsigned m_width = 0;
	unsigned m_height = 0;

	bool m_useImGui = true;

	std::unique_ptr<Camera> m_camera = nullptr;
};

