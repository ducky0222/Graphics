#pragma once

#include "ConstantBuffer.h"
#include "IBindable.h"
#include "IDrawable.h"

class TransformCBuf : public IBindable
{
public:
	TransformCBuf(FlatGraphics& graphics, unsigned slot = 0);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;
	void InitReference(const IDrawable& parent) override;

protected:
	struct Transforms
	{
		DirectX::SimpleMath::Matrix model;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		DirectX::SimpleMath::Matrix tex;
		DirectX::SimpleMath::Matrix modelInvTranspose;
	};

	void updateBindImpl(FlatGraphics& graphics, const Transforms& transform);
	Transforms getTransforms(FlatGraphics& graphics);

private:
	std::shared_ptr<VertexConstantBuffer<Transforms>> m_vertexConstantBuffer = nullptr;
	const IDrawable* m_parent = nullptr;

};

