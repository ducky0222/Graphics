#pragma once

#include "IBindable.h"
#include "FlatGraphics.h"
#include "ResourceManager.h"

template<typename T>
class ConstantBuffer : public IBindable
{
	static_assert(sizeof(T) % 16 == 0, "constant buffer must be aligned by 16 bytes");
public:
	ConstantBuffer(FlatGraphics& graphics, unsigned slot = 0);
	ConstantBuffer(FlatGraphics& graphics, const T& consts, unsigned slot = 0);
	virtual ~ConstantBuffer() = default;

	void Update(FlatGraphics& graphics, const T& consts);

protected:
	winrt::com_ptr<ID3D11Buffer> m_constantBuffer = nullptr;
	unsigned m_slot = 0;
};

#pragma region ConstantBufferFunc
template<typename T>
void ConstantBuffer<T>::Update(FlatGraphics& graphics, const T& consts)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub = {};

	GetContext(graphics)->Map(m_constantBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);

	memcpy(mappedSub.pData, &consts, sizeof(consts));

	GetContext(graphics)->Unmap(m_constantBuffer.get(), 0);
}

template<typename T>
ConstantBuffer<T>::ConstantBuffer(FlatGraphics& graphics, const T& consts, unsigned slot /*= 0*/)
	: m_slot(slot)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof(consts);
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = &consts;
	GetDevice(graphics)->CreateBuffer(&desc, &subData, m_constantBuffer.put());
}

template<typename T>
ConstantBuffer<T>::ConstantBuffer(FlatGraphics& graphics, unsigned slot /*= 0*/)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof(T);
	desc.StructureByteStride = 0;

	HRESULT hr = GetDevice(graphics)->CreateBuffer(&desc, NULL, m_constantBuffer.put());
	assert(SUCCEEDED(hr));
}
#pragma	endregion

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::ConstantBuffer;
	using ConstantBuffer<T>::m_constantBuffer;
	using ConstantBuffer<T>::m_slot;
	using IBindable::GetContext;

public:
	static std::shared_ptr<VertexConstantBuffer<T>> Create(FlatGraphics& graphics, const T& consts, unsigned slot = 0);
	static std::shared_ptr<VertexConstantBuffer<T>> Create(FlatGraphics& graphics, unsigned slot = 0);
	static std::string GenerateUID(const T&, unsigned slot);
	static std::string GenerateUID(unsigned slot = 0);

	virtual void Bind(FlatGraphics& graphics) override;

	string GetUID() const override;		
};

#pragma region VertexConstantBufferFunc
template<typename T>
string VertexConstantBuffer<T>::GetUID() const
{
	return GenerateUID(m_slot);
}

template<typename T>
void VertexConstantBuffer<T>::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->VSGetConstantBuffers(m_slot, 1, m_constantBuffer.put());
}

template<typename T>
std::string VertexConstantBuffer<T>::GenerateUID(unsigned slot /*= 0*/)
{
	return typeid(VertexConstantBuffer).name() + "#"s + to_string(slot);
}

template<typename T>
std::string VertexConstantBuffer<T>::GenerateUID(const T&, unsigned slot)
{
	return GenerateUID(slot);
}

template<typename T>
std::shared_ptr<VertexConstantBuffer<T>> VertexConstantBuffer<T>::Create(FlatGraphics& graphics, unsigned slot /*= 0*/)
{
	return ResourceManager::Create<VertexConstantBuffer>(graphics, slot);
}

template<typename T>
std::shared_ptr<VertexConstantBuffer<T>> VertexConstantBuffer<T>::Create(FlatGraphics& graphics, const T& consts, unsigned slot /*= 0*/)
{
	return ResourceManager::Create<VertexConstantBuffer>(graphics, consts, slot);
}
#pragma	endregion


#pragma region PixelConstantBufferFunc

#pragma	endregion
