#include "pch.h"
#include "FullScreenPass.h"

#include "FlatGraphics.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "Topology.h"
#include "RasterizerState.h"
#include "InputLayout.h"

FullScreenPass::FullScreenPass(FlatGraphics& graphics, std::string name)
	: BindingPass(std::move(name))
{
	Dvtx::VertexLayout lay;
	lay.Append(Dvtx::VertexLayout::Position2D);
	Dvtx::VertexBuffer bufFull{ lay };
	bufFull.EmplaceBack(DirectX::SimpleMath::Vector2{ -1, 1 });
	bufFull.EmplaceBack(DirectX::SimpleMath::Vector2{ 1, 1 });
	bufFull.EmplaceBack(DirectX::SimpleMath::Vector2{ -1, -1 });
	bufFull.EmplaceBack(DirectX::SimpleMath::Vector2{ 1, -1 });
	AddBind(VertexBuffer::Create(graphics, "$Full", std::move(bufFull)));

	std::vector<unsigned> indices = 
	{
		0,1,2,
		1,3,2
	};
	AddBind(IndexBuffer::Create(graphics, "$Full123", std::move(indices)));

	auto vs = VertexShader::Create(graphics, "../Resources/Shaders/Fullscreen_VS.hlsl");
	AddBind(InputLayout::Create(graphics, lay, vs->GetByteCode()));
	AddBind(std::move(vs));
	AddBind(Topology::Create(graphics));
	AddBind(RasterizerState::Create(graphics, false));
}

void FullScreenPass::Execute(FlatGraphics& graphics) const
{
	BindAll(graphics);

	graphics.DrawIndexed(6);
}
