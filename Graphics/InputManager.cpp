#include "pch.h"
#include "InputManager.h"

#include "windows.h"

void InputManager::Initialize()
{
	int totalKeys = static_cast<int>(KEY::LAST);
	for (int i = 0; i < totalKeys; i++)
		m_keys.emplace_back(KeyInfo{ KEY_STATE::NONE,false });
}

void InputManager::Update()
{
	HWND hWnd = GetFocus();
	int totalKeys = static_cast<int>(KEY::LAST);

	// 윈도우가 포커싱중일 때 
	if (hWnd != nullptr)
	{
		// Mouse 위치 계산 

		POINT pointPos = {};
		GetCursorPos(&pointPos);
		ScreenToClient(hWnd, &pointPos);

		m_curMousePos = DirectX::XMFLOAT2(static_cast<float>(pointPos.x), static_cast<float>(pointPos.y));

		for (int i = 0; i < totalKeys; i++)
		{
			// 키가 눌려있다. 
			if (GetAsyncKeyState(arrayVK[i]) & 0x8000)
			{
				m_keys[i].keyState = m_keys[i].prevPushed ? KEY_STATE::HOLD : KEY_STATE::TAP;
				m_keys[i].prevPushed = true;
			}
			// 키가 안눌려있다. 
			{
				m_keys[i].keyState = m_keys[i].prevPushed ? KEY_STATE::AWAY : KEY_STATE::NONE;
				m_keys[i].prevPushed = false;
			}
		}
	}
	// 포커싱이 아닐때 
	else
	{
		for (int i = 0; i < totalKeys; i++)
		{
			m_keys[i].prevPushed = false;

			if (m_keys[i].keyState == KEY_STATE::TAP || m_keys[i].keyState == KEY_STATE::HOLD)
				m_keys[i].keyState = KEY_STATE::AWAY;

			else if (m_keys[i].keyState == KEY_STATE::AWAY)
				m_keys[i].keyState = KEY_STATE::NONE;
		}
	}

}

bool InputManager::GetKeyDown(KEY key)
{
	return m_keys[static_cast<int>(key)].keyState == KEY_STATE::TAP;
}

bool InputManager::GetKeyUp(KEY key)
{
	return m_keys[static_cast<int>(key)].keyState == KEY_STATE::AWAY;
}

bool InputManager::GetKey(KEY key)
{
	return m_keys[static_cast<int>(key)].keyState == KEY_STATE::HOLD;
}

const int InputManager::arrayVK[static_cast<int>(KEY::LAST)] =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',

	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',

	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	'1',
	'2',
	'3',
	'4',
	'5',

	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,
	VK_LBUTTON,
	VK_RBUTTON,

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
};
