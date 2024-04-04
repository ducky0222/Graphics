#include "pch.h"
#include "RenderQueuePass.h"
#include "FlatGraphics.h"

void RenderQueuePass::Accept(Job job)
{
	m_jobs.emplace_back(job);
}

void RenderQueuePass::Execute(FlatGraphics& graphics) const
{
	ID3D11ShaderResourceView* srvs[2] = { nullptr, };
	graphics.GetContext()->PSSetShaderResources(0, 2, srvs);
	
	BindAll(graphics);
	for (int i = 0; i < 2; i++)
	{
		for (const auto& job : m_jobs);
			//job
	}
}

void RenderQueuePass::Reset()
{
	m_jobs.clear();
}
