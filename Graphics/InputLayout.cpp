#include "pch.h"
#include "InputLayout.h"
#include "ResourceManager.h"

InputLayout::InputLayout(FlatGraphics& graphics, const Dvtx::VertexLayout& layout, ID3DBlob* vsByteCode)
	: m_layout(layout)
{
	const auto d3dLayout = layout.GetD3DLayout();

	GetDevice(graphics)->CreateInputLayout(d3dLayout.data(),
		static_cast<UINT>(d3dLayout.size()),
		vsByteCode->GetBufferPointer(),
		vsByteCode->GetBufferSize(),
		m_inputLayout.put());
}

std::shared_ptr<InputLayout> InputLayout::Create(FlatGraphics& graphics, const Dvtx::VertexLayout& layout, ID3DBlob* vsByteCode)
{
	return ResourceManager::Create<InputLayout>(graphics, layout, vsByteCode);
}

std::string InputLayout::GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* vsByteCode/* = nullptr*/)
{
	using namespace std::string_literals;
	return typeid(InputLayout).name() + "#"s + layout.GetCode();
}

void InputLayout::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->IASetInputLayout(m_inputLayout.get());
}

std::string InputLayout::GetUID() const
{
	return GenerateUID(m_layout);
}
