#pragma once

#include "IRenderer.h"
#include "FlatGraphics.h"
#include "Camera.h"
#include "Model.h"
#include "ForwardRenderGraph.h"

class FlatRenderer : public IRenderer
{
public:
	FlatRenderer() = default;
	virtual ~FlatRenderer() = default;
	FlatRenderer(const FlatRenderer&) = delete;
	FlatRenderer& operator=(const FlatRenderer&) = delete;

	// IRenderer을(를) 통해 상속됨
	void Initialize(int hWnd, unsigned width, unsigned height, bool useImGUI) override;
	void Destroy() override;
	void OnResize(unsigned width, unsigned height) override;
	void BeginRender() override;
	void Excute() override;
	void EndRender() override;
	bool Submit(ModelType type, std::string key) override;
	bool Create(ModelType type, std::string key, std::string path) override;

	Camera* GetCamera() { return m_camera.get(); }

private:
	bool createModel(std::string key, std::string path);

private:
	std::unique_ptr<FlatGraphics> m_graphics = nullptr;
	std::unique_ptr<ForwardRenderGraph> m_renderGraph = nullptr;
	std::map<std::string, std::unique_ptr<Model>> m_models;
	std::unique_ptr<Camera> m_camera = nullptr;
};

