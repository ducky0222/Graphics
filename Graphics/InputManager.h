#pragma once

#include <vector>
#include <directxtk/SimpleMath.h>

enum class KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY,
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,
	LBTN,
	RBTN,

	F1,
	F2,
	F3,
	F4,
	F5,

	LAST,// 키 마지막
};

struct KeyInfo
{
	KEY_STATE keyState;
	bool prevPushed;
};

class InputManager 
{
public:
	InputManager() = default;
	~InputManager() = default;

	void Initialize();
	void Update();

	bool GetKeyDown(KEY key);
	bool GetKeyUp(KEY key);
	bool GetKey(KEY key);

	/// Getter
	DirectX::SimpleMath::Vector2 GetMousePos() const { return m_curMousePos; }
	std::vector<KeyInfo> GetKeys() const { return m_keys; }

public:
	static const int arrayVK[static_cast<int>(KEY::LAST)];

private:
	std::vector<KeyInfo> m_keys;
	DirectX::SimpleMath::Vector2 m_curMousePos;
};

