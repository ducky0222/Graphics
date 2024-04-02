#include "pch.h"
#include "VertexBuffer.h"
#include "ResourceManager.h"

VertexBuffer::VertexBuffer(FlatGraphics& graphics, const std::string& tag, const Dvtx::VertexBuffer& vbuf, D3D11_BIND_FLAG bindFlag /*= D3D11_BIND_VERTEX_BUFFER*/)
	: m_stride(static_cast<unsigned>(vbuf.GetLayout().Size())),
	m_tag(tag), m_layout(vbuf.GetLayout())
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | bindFlag;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = static_cast<UINT>(vbuf.SizeBytes());
	desc.StructureByteStride = m_stride;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = vbuf.GetData();

	GetDevice(graphics)->CreateBuffer(&desc, &subData, m_vertexBuffer.put());
}

VertexBuffer::VertexBuffer(FlatGraphics& graphics, const Dvtx::VertexBuffer& vbuf, D3D11_BIND_FLAG bindFlag /*= D3D11_BIND_VERTEX_BUFFER*/)
	: VertexBuffer(graphics, "?", vbuf)
{

}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(FlatGraphics& graphics, const std::string& tag, const Dvtx::VertexBuffer& vbuf)
{
	assert(tag != "?");
	return ResourceManager::Create<VertexBuffer>(graphics, tag, vbuf);
}

void VertexBuffer::Bind(FlatGraphics& graphics)
{
	const unsigned offset = 0;
	GetContext(graphics)->IASetVertexBuffers(0, 1, m_vertexBuffer.put(), &m_stride, &offset);
}

std::string VertexBuffer::GetUID() const
{
	return GenerateUID(m_tag);
}

std::string VertexBuffer::generateUID(const std::string& tag)
{
	using namespace std::string_literals;
	return typeid(VertexBuffer).name() + "#"s + tag;
}
