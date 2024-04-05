#include "pch.h"
#include "PixelShader.h"
#include "Utils.h"
#include "ResourceManager.h"

PixelShader::PixelShader(FlatGraphics& graphics, const std::string& path)
{
	winrt::com_ptr<ID3DBlob> blob;

	HRESULT hr = Utils::CompileShaderFromFile(Utils::ToWide(path).c_str(), "main", "ps_5_0", blob.put());

	if (FAILED(hr))
		hr = D3DReadFileToBlob(Utils::ToWide(path).c_str(), blob.put());

	assert(SUCCEEDED(hr));

	hr = GetDevice(graphics)->CreatePixelShader
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		m_pixelShader.put()
	);

	assert(SUCCEEDED(hr));
}

std::shared_ptr<PixelShader> PixelShader::Create(FlatGraphics& graphics, const std::string& path)
{
	return ResourceManager::Create<PixelShader>(graphics, path);
}

std::string PixelShader::GenerateUID(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(PixelShader).name() + "#"s + path;
}

void PixelShader::Bind(FlatGraphics& graphics)
{
	GetContext(graphics)->PSSetShader(m_pixelShader.get(), nullptr, 0);
}

std::string PixelShader::GetUID() const
{
	return GenerateUID(m_path);
}
