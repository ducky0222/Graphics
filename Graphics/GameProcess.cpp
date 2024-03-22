#include "GameProcess.h"
#include "FlatGraphics.h"
#include "TimeManager.h"
#include "InputManager.h"

#include <imgui_impl_win32.h>

FlatGraphics* GameProcess::m_flatGraphicsInstance = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT GameProcess::Initialize(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"D3DDemo";
	wcex.hIconSm = NULL;

	// 윈도우 운영체제에 등록
	RegisterClassExW(&wcex);

	// 애플리케이션 초기화를 수행합니다:
	m_hWnd = CreateWindowW(L"D3DDemo", L"D3DDemo", WS_OVERLAPPEDWINDOW,
		300, 100, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd)
	{
		return E_FAIL;
	}

	/// Graphics
	m_flatGraphics = new FlatGraphics;

	m_flatGraphicsInstance = m_flatGraphics;

	// 윈도우를 화면에 띄운다 / 갱신한다
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	// 클라이언트 영역이 원하는 게임 해상도가 되도록 윈도 사이즈 재조정
	recalcWindowSize();

	/// Managers
	m_timeManager = new TimeManager;
	m_timeManager->Initialize();

	m_inputManager = new InputManager;
	m_inputManager->Initialize();

	return S_OK;
}

void GameProcess::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
				break;

			DispatchMessage(&m_msg);
		}
		else
		{
			updateAll();
			renderAll();
		}
	}
}

void GameProcess::Finalize()
{
	delete m_flatGraphics;
	delete m_inputManager;
	delete m_timeManager;
}

void GameProcess::updateAll()
{
	m_timeManager->Update();
	m_deltaTime = m_timeManager->GetDeltaTime();
	m_inputManager->Update();
}

void GameProcess::renderAll()
{

}

void GameProcess::recalcWindowSize()
{
	RECT nowRect{};
	DWORD style = static_cast<DWORD>(GetWindowLong(m_hWnd, GWL_STYLE));
	DWORD exStyle = static_cast<DWORD>(GetWindowLong(m_hWnd, GWL_EXSTYLE));

	GetWindowRect(m_hWnd, &nowRect);

	RECT newRect = { 0,0,m_screenWidth,m_screenHeight };

	AdjustWindowRectEx(&newRect, style, NULL, exStyle);

	// 클라이언트 영역보다 윈도 크기는 더 커야 한다. (외곽선, 타이틀 등)
	int newWidth = (newRect.right - newRect.left);
	int newHeight = (newRect.bottom - newRect.top);

	SetWindowPos(m_hWnd, HWND_NOTOPMOST, nowRect.left, nowRect.top, newWidth, newHeight, SWP_SHOWWINDOW);

}


LRESULT CALLBACK GameProcess::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	HDC hdc{};
	PAINTSTRUCT ps{};
	int data = 0;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//m_pGraphicsEngineInstance->OnMouseDown(/*wParam, */LOWORD(lParam), HIWORD(lParam));
		//return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		//m_pGraphicsEngineInstance->OnMouseUp(/*wParam, */LOWORD(lParam), HIWORD(lParam));
		//return 0;

	case WM_MOUSEMOVE:
		data = static_cast<int>(wParam);
		//m_pGraphicsEngineInstance->OnMouseMove(data, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
