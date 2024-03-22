#include "TimeManager.h"

#include <string>

void TimeManager::Initialize()
{
	// ���� ī��Ʈ
	QueryPerformanceCounter(&m_prevCount);

	// �ʴ� ī��Ʈ Ƚ��
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
			// FPS�� ���� Ȱ�� �������� Ÿ��Ʋ�ٿ� ���
			std::wstring title = L"FPS: " + std::to_wstring(m_lastFPS);
			auto fpsText = SetWindowText(hWnd, title.c_str());
		}
	}
}
