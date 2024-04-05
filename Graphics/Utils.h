#pragma once

class Utils
{
public:
	template<typename Iter>
	static void SplitStringIter(const std::string& str, const std::string& delim, Iter out)
	{
		if (delim.empty())
			*out++ = str;
		else
		{
			size_t a = 0, b = str.find(delim);
			for (; b != std::string::npos; a = b + delim.length(), b = str.find(delim, a))
			{
				*out++ = std::move(str.substr(a, b - a));
			}
			*out++ = std::move(str.substr(a, str.length() - a));
		}
	}

	static std::vector<std::string> SplitString(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> strings;
		SplitStringIter(str, delim, std::back_inserter(strings));
		return strings;
	}

	static std::wstring ToWide(const std::string& str)
	{
		int num_chars = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
		std::wstring wstrTo;
		if (num_chars)
		{
			wstrTo.resize(num_chars);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), &wstrTo[0], num_chars);
		}
		return wstrTo;
	}

	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint, ShaderModel,
			dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
				pErrorBlob->Release();
			}
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}
};

