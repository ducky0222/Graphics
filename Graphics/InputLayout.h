#pragma once

#include "IBindable.h"
#include "Vertex.h"

class InputLayout : public IBindable
{
public:
	InputLayout(FlatGraphics& graphics, const Dvtx::VertexLayout& layout, ID3DBlob* vsByteCode);

	static std::shared_ptr<InputLayout> Create(FlatGraphics& graphics, const Dvtx::VertexLayout& layout, ID3DBlob* vsByteCode);
	static std::string GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* vsByteCode = nullptr);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;
	std::string GetUID() const override;

	const Dvtx::VertexLayout& GetLayout() const { return m_layout; }
protected:
	Dvtx::VertexLayout m_layout;
	winrt::com_ptr<ID3D11InputLayout> m_inputLayout = nullptr;

};

