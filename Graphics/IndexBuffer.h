#pragma once

#include "IBindable.h"
#include "ResourceManager.h"

class FlatGraphics;

class IndexBuffer : public IBindable
{
public:
	IndexBuffer(FlatGraphics& graphics, const std::vector<unsigned>& indices);
	IndexBuffer(FlatGraphics& graphics, std::string tag, const std::vector<unsigned>& indices);
	~IndexBuffer() = default;

	static std::shared_ptr<IndexBuffer> Create(FlatGraphics& graphics, const std::string& tag, const std::vector<unsigned>& indices);
	template<typename... Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&... ignore);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	unsigned GetCount() const { return m_count; }

private:
	static std::string generateUID(const std::string& tag);

protected:
	std::string m_tag = "?";
	unsigned m_count = 0;
	winrt::com_ptr<ID3D11Buffer> m_indexBuffer = nullptr;

};

template<typename... Ignore>
std::string IndexBuffer::GenerateUID(const std::string& tag, Ignore&... ignore)
{
	return generateUID(tag);
}

