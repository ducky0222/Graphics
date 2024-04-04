#pragma once
#include "BindingPass.h"
#include "Job.h"

class RenderQueuePass : public BindingPass
{
public:
	RenderQueuePass(std::string name, std::vector<std::shared_ptr<IBindable>> binds = {}, bool useAlpha = false)
		: BindingPass(std::move(name), std::move(binds)), m_alphaUsed(useAlpha)
	{}
	virtual ~RenderQueuePass() { m_jobs.clear(); }

	void Accept(Job job);
	void Execute(FlatGraphics& graphics) const override;
	void Reset() override;

protected:
	std::vector<Job> m_jobs;
	bool m_alphaUsed = false;
};

