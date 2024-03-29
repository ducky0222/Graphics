#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(FlatGraphics& graphics, const std::vector<unsigned>& indices)
	: IndexBuffer(graphics, "?", indices)
{

}

IndexBuffer::IndexBuffer(FlatGraphics& graphics, std::string tag, const std::vector<unsigned>& indices)
	: m_tag(tag), m_count(static_cast<unsigned>(indices.size()))
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = m_count * sizeof(unsigned);
	desc.StructureByteStride = sizeof(unsigned);

	D3D11_SUBRESOURCE_DATA iData = {};
	iData.pSysMem = indices.data();

	GetDevice(graphics)->CreateBuffer(&desc, &iData, m_indexBuffer.put());
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(FlatGraphics& graphics, const std::string& tag, const vector<unsigned>& indices)
{
	return ResourceManager::Create<IndexBuffer>(graphics, tag, indices);
}

void IndexBuffer::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);
}

std::string IndexBuffer::generateUID(const std::string& tag)
{
	return typeid(IndexBuffer).name() + "#"s + tag;
}
