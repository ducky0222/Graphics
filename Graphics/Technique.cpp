#include "pch.h"
#include "Technique.h"

Technique::Technique(std::string name, bool startActive /*= true*/)
	: m_isActive(startActive), m_name(name)
{

}

void Technique::Submit(IDrawable& drawable) const
{
	if (m_isActive)
	{
		for (const auto& step : m_steps)
			step.Submit(drawable);
	}
}

void Technique::Link(RenderGraph& renderGraph)
{
	for (auto& step : m_steps)
		step.Link(renderGraph);
}

void Technique::InitReference(const IDrawable& parent)
{
	for (auto& step : m_steps)
		step.InitReferences(parent);
}

void Technique::AddStep(Step step)
{
	m_steps.emplace_back(std::move(step));
}

void Technique::SetActiveState(bool activeIn)
{
	m_isActive = activeIn;
}
