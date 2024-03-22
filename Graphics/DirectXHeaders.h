#pragma once

#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <winrt/windows.foundation.h>
#include <dxgi.h>
#include <directxtk/DDSTextureLoader.h>
#include <directxtk/WICTextureLoader.h>
#include <DirectXTex.h>
#include <DirectXTex.inl>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "../lib/DirectXTex.lib")
#endif