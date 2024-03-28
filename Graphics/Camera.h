#pragma once

#include "DirectXHeaders.h"

class Camera
{
public:
	Camera();
	~Camera() = default;
	Camera(const Camera&) = default;
	Camera& operator=(const Camera&) = default;

	static DirectX::SimpleMath::Vector3 GetCameraPosition(const DirectX::SimpleMath::Matrix& view);

	void UpdateViewMatrix();
	void LookAt(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& up);

	void RotatePitch(float angle);
	void RotateYaw(float angle);
	void RotateRoll(float angle);
	void RotateAxis(const DirectX::SimpleMath::Vector3& axis, float angle);

	void TranslateLook(float distance);
	void TranslateRight(float distance);
	void TranslateUp(float distance);

	inline void SetLens(float fovY, float aspect, float zn, float zf);
	inline void SetPosition(float x, float y, float z);
	inline void SetPosition(const DirectX::SimpleMath::Vector3& position);

	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetRight() const { return m_right; }
	DirectX::SimpleMath::Vector3 GetUp() const { return m_up; }
	DirectX::SimpleMath::Vector3 GetLook() const { return m_look; }

	float GetNearZ() const { return m_nearZ; }
	float GetFarZ() const { return m_farZ; }
	float GetAspect() const { return m_aspect; }
	float GetFovY() const { return m_fovY; }
	inline float GetFovX() const;

	float GetNearWindowHeight() const { return m_nearWindowHeight; }
	inline float GetNearWindowWidth() const;
	float GetFarWindowHeight() const { return m_farWindowHeight; }
	inline float GetFarWindowWidth() const;

	DirectX::SimpleMath::Matrix GetView() const { return m_view; }
	DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
	inline DirectX::SimpleMath::Matrix GetViewProjection() const;

private:
	DirectX::SimpleMath::Vector3 m_position = {0.0f, 0.0f, 0.0f};
	DirectX::SimpleMath::Vector3 m_right = {1.0f, 0.0f, 0.0f};
	DirectX::SimpleMath::Vector3 m_up = {0.0f, 1.0f, 0.0f};
	DirectX::SimpleMath::Vector3 m_look = {0.0f, 0.0f, 1.0f};

	float m_nearZ = 0.0f;
	float m_farZ = 0.0f;
	float m_aspect = 0.0f;
	float m_fovY = 0.0f;
	float m_nearWindowHeight = 0.0f;
	float m_farWindowHeight = 0.0f;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

};

