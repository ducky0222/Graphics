#include "TimeManager.h"

#include <string>

void TimeManager::Initialize()
{
	// 현재 카운트
	QueryPerformanceCounter(&m_prevCount);

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&m_frequency);
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&m_curCount);
	m_deltaTime = static_cast<double>(m_curCount.QuadPart - m_prevCount.QuadPart) / static_cast<double>(m_frequency.QuadPart);
	m_prevCount = m_curCount;

	m_fps++;

	if (m_elapsedTime > 1.0)
	{
		m_elapsedTime -= 1.0;
		m_lastFPS = m_fps;
		m_fps = 0;

		HWND hWnd = GetForegroundWindow();
		if (hWnd)
		{
			// FPS를 현재 활성 윈도우의 타이틀바에 출력
			std::wstring title = L"FPS: " + std::to_wstring(m_lastFPS);
			auto fpsText = SetWindowText(hWnd, title.c_str());
		}
	}
}
