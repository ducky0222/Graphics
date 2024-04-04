#pragma once

#include "Step.h"

class IDrawable;
class RenderGraph;

class Technique
{
public:
	Technique() = default;
	Technique(std::string name, bool startActive = true);
	~Technique() = default;

	void Submit(IDrawable& drawable) const;
	void Link(RenderGraph& renderGraph);

	void InitReference(const IDrawable& parent);

	void AddStep(Step step);

	std::vector<Step>& GetSteps() { return m_steps; }

	void SetActiveState(bool activeIn);

	bool IsActive() const { return m_isActive; }
	const std::string& GetName() const { return m_name; }

private:
	bool m_isActive = true;
	std::vector<Step> m_steps;
	std::string m_name;
};

