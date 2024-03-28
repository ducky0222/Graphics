#include "Camera.h"

constexpr float PI = 3.1415927f;

Camera::Camera()
{
	SetLens(0.25f * PI, 1.0f, 1.0f, 1000.0f);
}

DirectX::SimpleMath::Vector3 Camera::GetCameraPosition(const DirectX::SimpleMath::Matrix& view)
{
	DirectX::SimpleMath::Vector3 cameraPosition =
		-view._41 * DirectX::SimpleMath::Vector3(view._11, view._21, view._31) +
		-view._42 * DirectX::SimpleMath::Vector3(view._12, view._22, view._32) +
		-view._43 * DirectX::SimpleMath::Vector3(view._13, view._23, view._33);

	return cameraPosition;
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMVECTOR right = XMLoadFloat3(&m_right);
	DirectX::XMVECTOR up = XMLoadFloat3(&m_up);
	DirectX::XMVECTOR look = XMLoadFloat3(&m_look);
	DirectX::XMVECTOR position = XMLoadFloat3(&m_position);

	look = DirectX::XMVector3Normalize(look);
	up = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(look, right));

	right = DirectX::XMVector3Cross(up, look);

	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, right));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, up));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, look));

	DirectX::XMStoreFloat3(&m_right, right);
	DirectX::XMStoreFloat3(&m_up, up);
	DirectX::XMStoreFloat3(&m_look, look);

	m_view(0, 0) = m_right.x;
	m_view(1, 0) = m_right.y;
	m_view(2, 0) = m_right.z;
	m_view(3, 0) = x;
	
	m_view(0, 1) = m_up.x;
	m_view(1, 1) = m_up.y;
	m_view(2, 1) = m_up.z;
	m_view(3, 1) = y;
	
	m_view(0, 2) = m_look.x;
	m_view(1, 2) = m_look.y;
	m_view(2, 2) = m_look.z;
	m_view(3, 2) = z;
	
	m_view(0, 3) = 0.0f;
	m_view(1, 3) = 0.0f;
	m_view(2, 3) = 0.0f;
	m_view(3, 3) = 1.0f;
}

void Camera::LookAt(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& worldUp)
{
	DirectX::XMVECTOR look = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target, pos));
	DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, look));
	DirectX::XMVECTOR up = DirectX::XMVector3Cross(look, right);

	DirectX::XMStoreFloat3(&m_position, pos);
	DirectX::XMStoreFloat3(&m_look, look);
	DirectX::XMStoreFloat3(&m_right, right);
	DirectX::XMStoreFloat3(&m_up, up);
}

void Camera::RotatePitch(float angle)
{
	DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(m_right, angle);

	m_up = DirectX::SimpleMath::Vector3::TransformNormal(m_up, rotationMatrix);
	m_look = DirectX::SimpleMath::Vector3::TransformNormal(m_look, rotationMatrix);
}

void Camera::RotateYaw(float angle)
{
	DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(m_up, angle);

	m_right = DirectX::SimpleMath::Vector3::TransformNormal(m_right, rotationMatrix);
	m_look = DirectX::SimpleMath::Vector3::TransformNormal(m_look, rotationMatrix);
}

void Camera::RotateRoll(float angle)
{
	DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(m_look, angle);

	m_up = DirectX::SimpleMath::Vector3::TransformNormal(m_up, rotationMatrix);
	m_right = DirectX::SimpleMath::Vector3::TransformNormal(m_right, rotationMatrix);
}

void Camera::RotateAxis(const DirectX::SimpleMath::Vector3& axis, float angle)
{
	assert(axis.LengthSquared() <= 1.001f);

	DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(axis, angle);

	m_up = DirectX::SimpleMath::Vector3::TransformNormal(m_up, rotationMatrix);
	m_right = DirectX::SimpleMath::Vector3::TransformNormal(m_right, rotationMatrix);
	m_look = DirectX::SimpleMath::Vector3::TransformNormal(m_look, rotationMatrix);
}

void Camera::TranslateLook(float distance)
{
	DirectX::SimpleMath::Vector3 movedPosition = m_look * distance;
	m_position += movedPosition;
}

void Camera::TranslateRight(float distance)
{
	DirectX::SimpleMath::Vector3 movedPosition = m_right * distance;
	m_position += movedPosition;
}

void Camera::TranslateUp(float distance)
{
	DirectX::SimpleMath::Vector3 movedPosition = m_up * distance;
	m_position += movedPosition;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = zn;
	m_farZ = zf;

	m_nearWindowHeight = 2.0f * m_nearZ * tanf(fovY * 0.5f);
	m_farWindowHeight = 2.0f * m_farZ * tanf(fovY * 0.5f);

	m_projection = DirectX::XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position = { x, y, z };
}

void Camera::SetPosition(const DirectX::SimpleMath::Vector3& position)
{
	m_position = position;
}

float Camera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();

	return 2.0f * atan(halfWidth / m_nearZ);
}

float Camera::GetNearWindowWidth() const
{
	return m_aspect * m_nearWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return m_aspect * m_farWindowHeight;
}

DirectX::SimpleMath::Matrix Camera::GetViewProjection() const
{
	return m_view * m_projection;
}
