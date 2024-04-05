#pragma once

#include "IBindable.h"

class VertexShader : public IBindable
{
public:
	VertexShader(FlatGraphics& graphics, const std::string& path);
	virtual ~VertexShader() = default;

	static std::shared_ptr<VertexShader> Create(FlatGraphics& graphics, const std::string& path);
	static std::string GenerateUID(const std::string& path);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	ID3D10Blob* GetByteCode() const;
	std::string GetUID() const override;

private:
	std::string m_path;
	winrt::com_ptr<ID3D10Blob> m_byteCodeBlob = nullptr;
	winrt::com_ptr<ID3D11VertexShader> m_vertexShader = nullptr;

};

