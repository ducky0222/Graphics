#include "pch.h"
#include "FlatRenderer.h"
#include "ResourceManager.h"

void FlatRenderer::Initialize(int hWnd, unsigned width, unsigned height, bool useImGUI)
{
	m_graphics = std::make_unique<FlatGraphics>();
	m_renderGraph = std::make_unique<ForwardRenderGraph>(*m_graphics);
	m_camera = std::make_unique<Camera>();

	m_graphics->Initialize(reinterpret_cast<HWND>(hWnd), width, height, useImGUI);
}

void FlatRenderer::Destroy()
{
	m_models.clear();

	m_graphics = nullptr;
	m_renderGraph = nullptr;

	ResourceManager::Get().Destroy();
}

void FlatRenderer::OnResize(unsigned width, unsigned height)
{
	width = max(width, 200);
	height = max(height, 200);
}

void FlatRenderer::BeginRender()
{
	m_camera->UpdateViewMatrix();
}

void FlatRenderer::Excute()
{
	m_renderGraph->Execute(*m_graphics);

}

void FlatRenderer::EndRender()
{
	m_graphics->EndFrame();
	m_renderGraph->Reset();

	auto resetMapLambda = [](auto& modelMap)
		{
			for (auto& model : modelMap)
				model.second->Reset();
		};

	resetMapLambda(m_models);
}

bool FlatRenderer::Submit(ModelType type, std::string key)
{
	auto submitLambda = [](auto& mapContainer, std::string keyStr)
		{
			auto findIt = mapContainer.find(keyStr);

			if (findIt == mapContainer.end())
				return false;

			findIt->second->Submit();

			return true;
		};

	switch (type)
	{
	case ModelType::BASIC:
		return submitLambda(m_models, key);
		break;
	case ModelType::SKINNED:
		break;
	case ModelType::GEOMETRY:
		break;
	default:
		break;
	}
}

bool FlatRenderer::Create(ModelType type, std::string key, std::string path)
{
	switch (type)
	{
	case ModelType::BASIC:
		createModel(key, path);
		break;
	case ModelType::SKINNED:
		break;
	case ModelType::GEOMETRY:
		assert(false);
		break;
	default:
		assert(false);
		break;
	}

	return false;
}

bool FlatRenderer::createModel(std::string key, std::string path)
{
	auto findIt = m_models.find(key);

	if (findIt != m_models.end())
		return false;

	m_models.insert({ key, std::make_unique<Model>(*m_graphics, path) });
	m_models[key]->LinkTechniques(*m_renderGraph);
}
