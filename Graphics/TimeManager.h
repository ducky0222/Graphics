#pragma once

#include <stdint.h>

class TimeManager
{
public:
	TimeManager() = default;
	~TimeManager() = default;

	void Initialize();
	void Update();

	float GetDeltaTime() const { return static_cast<float>(m_deltaTime); }
	uint32_t GetFPS() const { return m_lastFPS; }

private:
	LARGE_INTEGER m_curCount = {};
	LARGE_INTEGER m_prevCount = {};
	LARGE_INTEGER m_frequency = {};

	double m_deltaTime = 0.0f;
	double m_elapsedTime = 0.0;
	uint32_t m_lastFPS = 0;
	uint32_t m_fps = 0;
};

