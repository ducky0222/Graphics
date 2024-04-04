#include "pch.h"
#include "Step.h"
#include "IDrawable.h"
#include "RenderGraph.h"
#include "FlatGraphics.h"
#include "RenderQueuePass.h"

Step::Step(std::string targetPassName)
	: m_targetPassName{ std::move(targetPassName) }
{

}

void Step::Bind(FlatGraphics& graphics) const
{
	for (const auto& bindable : m_bindables)
		bindable->Bind(graphics);
}

void Step::Submit(IDrawable& drawable) const
{
	assert(m_targetPass != nullptr);
	//m_targetPass->Accept(Job{ this, &drawable });
}

void Step::InitReferences(const IDrawable& parent)
{
	for (auto& bindable : m_bindables)
		bindable->InitReference(parent);
}

void Step::Link(RenderGraph& renderGraph)
{
	assert(m_targetPass == nullptr);
	m_targetPass = &renderGraph.GetRenderQueue(m_targetPassName);
}

void Step::AddBindable(std::shared_ptr<IBindable> bind)
{
	m_bindables.emplace_back(std::move(bind));
}
