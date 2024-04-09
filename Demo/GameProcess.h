#pragma once

#include "WindowsHeader.h"

class IRenderer;
class FlatGraphics;
class InputManager;
class TimeManager;

class GameProcess
{
public:
	GameProcess() = default;
	~GameProcess() = default;
	GameProcess(const GameProcess&) = delete;
	GameProcess& operator=(const GameProcess&) = delete;

	HRESULT Initialize(HINSTANCE hInstance);
	void Loop();
	void Finalize();

	// �޽��� �ڵ鷯(���� �ݹ�)
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
	FlatGraphics* m_flatGraphics = nullptr;
	IRenderer* m_renderer = nullptr;

	/// Managers
	TimeManager* m_timeManager = nullptr;
	InputManager* m_inputManager = nullptr;

	float m_deltaTime = 0.0f;

};

