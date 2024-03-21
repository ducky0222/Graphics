#pragma once

#include <windows.h>
#include <memory>

class FlatGraphics;
class InputManager;
class TimeManager;

class GameProcess
{
public:
	GameProcess() = default;
	~GameProcess() = default;

	HRESULT Initialize(HINSTANCE hInstance);
	void Loop();
	void Finalize();

	// 메시지 핸들러(윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void updateAll();
	void renderAll();

	// for Initialize
	void recalcWindowSize();

private:
	/// Initialize
	HWND m_hWnd = {};
	MSG m_msg = {};
	const int m_screenWidth = 1920;
	const int m_screenHeight = 1080;
	const int m_windowsPosX = 300;
	const int m_windowsPosY = 300;


	/// Graphics
	std::unique_ptr<FlatGraphics> m_flatGraphics = nullptr;
	static FlatGraphics* m_flatGraphicsInstance;

	/// Managers
	std::unique_ptr<TimeManager> m_timeManager = nullptr;
	std::unique_ptr<InputManager> m_inputManager = nullptr;

};

