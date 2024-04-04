#pragma once

class IDrawable;
class RenderQueuePass;
class RenderGraph;
class FlatGraphics;
class IBindable;

class Step
{
public:
	Step(std::string targetPassName);
	~Step() = default;
	
	void Bind(FlatGraphics& graphics) const;

	void Submit(IDrawable& drawable) const;
	void InitReferences(const IDrawable& parent);
	void Link(RenderGraph& renderGraph);

	void AddBindable(std::shared_ptr<IBindable> bind);

private:
	std::vector<std::shared_ptr<IBindable>> m_bindables;
	RenderQueuePass* m_targetPass = nullptr;
	std::string m_targetPassName;
};

