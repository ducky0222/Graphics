#include "pch.h"
#include "TransformCBuf.h"
#include "ResourceManager.h"

static DirectX::SimpleMath::Matrix InverseTranspose(const DirectX::SimpleMath::Matrix& matrix)
{
	DirectX::SimpleMath::Matrix result = matrix;
	result._41 = 0.f;
	result._42 = 0.f;
	result._43 = 0.f;
	result._44 = 1.f;

	DirectX::SimpleMath::Matrix invResult;
	result.Invert(invResult);

	return invResult.Transpose();
}

TransformCBuf::TransformCBuf(FlatGraphics& graphics, unsigned slot /*= 0*/)
{
	m_vertexConstantBuffer = VertexConstantBuffer<Transforms>::Create(graphics, slot);
}

void TransformCBuf::Bind(FlatGraphics& graphics)
{
	updateBindImpl(graphics, getTransforms(graphics));
}

void TransformCBuf::InitReference(const IDrawable& parent)
{
	m_parent = &parent;
}

void TransformCBuf::updateBindImpl(FlatGraphics& graphics, const Transforms& transform)
{
	assert(m_parent != nullptr);
	m_vertexConstantBuffer->Update(graphics, transform);
	m_vertexConstantBuffer->Bind(graphics);
}

TransformCBuf::Transforms TransformCBuf::getTransforms(FlatGraphics& graphics)
{
	assert(m_parent != nullptr);
	const auto modelView = m_parent->GetTransform() * graphics.GetView();

	// TODO 텍스처 변환 행렬
	Transforms result =
	{
		m_parent->GetTransform().Transpose(),
		graphics.GetView().Transpose(),
		graphics.GetProjection().Transpose(),
		m_parent->GetTexTransform().Transpose(),
		InverseTranspose(m_parent->GetTransform()).Transpose()
	};

	return result;
}
