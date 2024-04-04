#pragma once

class FlatGraphics;
class RenderTarget;
class DepthStencil;
class Pass;
class RenderQueuePass;
class Source;
class Sink;

class RenderGraph
{
public:
	RenderGraph(FlatGraphics& graphics);
	virtual ~RenderGraph() = default;

	virtual void Execute(FlatGraphics& graphics);
	void Reset();

	RenderQueuePass& GetRenderQueue(const std::string& passName);

protected:
	void finalize();

	void addGlobalSource(std::unique_ptr<Source> source);
	void addGlobalSink(std::unique_ptr<Sink> sink);
	void appendPass(std::unique_ptr<Pass> pass);

	void setSinkTarget(const std::string& sinkName, const std::string& target);

	Pass* findPassByNameOrNull(const std::string& name);

private:
	void linkSinks(Pass& pass);
	void linkGlobalSinks();

protected:
	std::vector<std::unique_ptr<Pass>> m_passes;
	std::vector<std::unique_ptr<Source>> m_globalSources;
	std::vector<std::unique_ptr<Sink>> m_globalSinks;

	std::shared_ptr<RenderTarget> m_backBuffer = nullptr;
	std::shared_ptr<RenderTarget> m_renderTarget = nullptr;
	std::shared_ptr<DepthStencil> m_masterDepth = nullptr;

	bool m_isFinalized = false;
};

