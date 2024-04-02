#pragma once

#include "IBindable.h"
#include "Vertex.h"

class VertexBuffer : public IBindable
{
public:
	VertexBuffer(FlatGraphics& graphics, const std::string& tag, const Dvtx::VertexBuffer& vbuf, D3D11_BIND_FLAG bindFlag = D3D11_BIND_VERTEX_BUFFER);
	VertexBuffer(FlatGraphics& graphics, const Dvtx::VertexBuffer& vbuf, D3D11_BIND_FLAG bindFlag = D3D11_BIND_VERTEX_BUFFER);

	static std::shared_ptr<VertexBuffer> Create(FlatGraphics& graphics, const std::string& tag, const Dvtx::VertexBuffer& vbuf);
	template<typename...Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&...ignore);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	std::string GetUID() const override;
	const Dvtx::VertexLayout& GetLayout() const { return m_layout; }

private:
	static std::string generateUID(const std::string& tag);

protected:
	std::string m_tag;
	unsigned m_stride = 0;
	winrt::com_ptr<ID3D11Buffer> m_vertexBuffer = nullptr;
	Dvtx::VertexLayout m_layout;

};

template<typename... Ignore>
std::string VertexBuffer::GenerateUID(const std::string& tag, Ignore&&... ignore)
{
	return generateUID(tag);
}

