#include "pch.h"
#include "VertexShader.h"
#include "Utils.h"
#include "ResourceManager.h"

VertexShader::VertexShader(FlatGraphics& graphics, const std::string& path)
{
	HRESULT hr = Utils::CompileShaderFromFile(Utils::ToWide(path).c_str(), "main", "vs_5_0", m_byteCodeBlob.put());

	if (FAILED(hr))
		hr = D3DReadFileToBlob(Utils::ToWide(path).c_str(), m_byteCodeBlob.put());

	assert(SUCCEEDED(hr));

	hr = GetDevice(graphics)->CreateVertexShader
	(
		m_byteCodeBlob->GetBufferPointer(),
		m_byteCodeBlob->GetBufferSize(),
		nullptr,
		m_vertexShader.put()
	);

	assert(SUCCEEDED(hr));
}

std::shared_ptr<VertexShader> VertexShader::Create(FlatGraphics& graphics, const std::string& path)
{
	return ResourceManager::Create<VertexShader>(graphics, path);
}

std::string VertexShader::GenerateUID(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + path;
}

void VertexShader::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->VSSetShader(m_vertexShader.get(), nullptr, 0);
}

ID3D10Blob* VertexShader::GetByteCode() const
{
	return m_byteCodeBlob.get();
}

std::string VertexShader::GetUID() const
{
	return GenerateUID(m_path);
}
