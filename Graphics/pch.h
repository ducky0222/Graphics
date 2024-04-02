#pragma once

/// STL
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <optional>
#include <filesystem>

/// Windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/// DX11
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

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

/// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "assimp-vc143-mtd.lib")

/// ImGUI
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuizmo.h>