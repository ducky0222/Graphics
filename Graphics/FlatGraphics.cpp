#include "pch.h"
#include "FlatGraphics.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Camera.h"

void FlatGraphics::Initialize(HWND hWnd, unsigned width, unsigned height, bool useImgui)
{
	m_width = width;
	m_height = height;
	m_useImGui = useImgui;

	unsigned createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		m_device.put(),
		&featureLevel,
		m_context.put());

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
	}

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(m_device.get(), &sd, m_swapChain.put());

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// 후면버퍼를 참조로하는 RenderTarget 생성
	winrt::com_ptr<ID3D11Texture2D> backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), backBuffer.put_void());
	m_backBuffer = std::shared_ptr<OutputOnlyRenderTarget>(new OutputOnlyRenderTarget(*this, backBuffer.get()));
	m_depthStencil = std::make_shared<OutputOnlyDepthStencil>(*this);
	m_renderTarget = std::make_shared<ShaderInputRenderTarget>(*this, m_width, m_height, 0, DXGI_FORMAT_R16G16B16A16_FLOAT);

	// 뷰포트 설정
	D3D11_VIEWPORT vp = CD3D11_VIEWPORT();
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_context->RSSetViewports(1, &vp);

	if (m_useImGui)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		static_cast<void>(io);

		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(m_device.get(), m_context.get());
	}
}

void FlatGraphics::Finalize() const
{
	if (m_useImGui)
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}
}

void FlatGraphics::OnResize(unsigned width, unsigned height)
{
	assert(m_device);
	assert(m_context);
	assert(m_swapChain);

	m_width = width;
	m_height = height;

	m_renderTarget->Release();
	m_backBuffer->Release();

	HRESULT hr = m_swapChain->ResizeBuffers(0, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	winrt::com_ptr<ID3D11Texture2D> backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), backBuffer.put_void());

	m_renderTarget->OnResize(*this, m_width, m_height);
	m_backBuffer->OnResize(*this, m_width, m_height);
	m_depthStencil->OnResize(*this);

	D3D11_VIEWPORT vp = CD3D11_VIEWPORT();
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_context->RSSetViewports(1, &vp);
}

void FlatGraphics::BeginFrame() const
{
	if (m_useImGui)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
}

void FlatGraphics::DrawIndexed(unsigned count)
{
	m_context->DrawIndexed(count, 0, 0);
}

void FlatGraphics::EndFrame()
{
	if (m_useImGui)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	m_swapChain->Present(1, 0);
}

