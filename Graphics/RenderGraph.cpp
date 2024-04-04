#include "pch.h"
#include "RenderGraph.h"
#include "Pass.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "RenderQueuePass.h"
#include "Sink.h"
#include "Source.h"
#include "Utils.h"
#include "FlatGraphics.h"


RenderGraph::RenderGraph(FlatGraphics& graphics)
	: m_renderTarget(graphics.GetRenderTarget())
	, m_backBuffer(graphics.GetBackBuffer())
	, m_masterDepth(graphics.GetDepthStencil())
{
	addGlobalSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));
	addGlobalSource(DirectBufferSource<RenderTarget>::Make("backBuffer", m_backBuffer));
	addGlobalSource(DirectBufferSource<DepthStencil>::Make("masterDepth", m_masterDepth));

	addGlobalSink(DirectBufferSink<RenderTarget>::Make("backBuffer", m_backBuffer));
}

void RenderGraph::Execute(FlatGraphics& graphics)
{
	assert(m_isFinalized);

	for (auto& pass : m_passes)
		pass->Execute(graphics);
}

void RenderGraph::Reset()
{
	assert(m_isFinalized);

	for (auto& pass : m_passes)
		pass->Reset();
}

RenderQueuePass& RenderGraph::GetRenderQueue(const std::string& passName)
{
	try
	{
		for (const auto& pass : m_passes)
		{
			if (pass->GetName() == passName)
				return dynamic_cast<RenderQueuePass&>(*pass);
		}
	}
	catch (std::bad_cast&)
	{
		assert(false);
	}
	assert(false);


}

void RenderGraph::finalize()
{
	assert(!m_isFinalized);

	for (const auto& pass : m_passes)
		pass->Finalize();

	linkGlobalSinks();
	m_isFinalized = true;
}

void RenderGraph::addGlobalSource(std::unique_ptr<Source> source)
{
	m_globalSources.emplace_back(std::move(source));
}

void RenderGraph::addGlobalSink(std::unique_ptr<Sink> sink)
{
	m_globalSinks.emplace_back(std::move(sink));
}

void RenderGraph::appendPass(std::unique_ptr<Pass> pass)
{
	assert(!m_isFinalized);

	// 랜더 그래프에는 동일한 패스가 존재할 수 없다.
	for (const auto& _pass : m_passes)
	{
		if (_pass->GetName() == pass->GetName())
			assert(false);
	}

	// 패스가 추가될 때마다 소스/싱크 매핑을 시행함
	linkSinks(*pass);
	m_passes.emplace_back(std::move(pass));
}

void RenderGraph::setSinkTarget(const std::string& sinkName, const std::string& target)
{
	const auto finder = [&sinkName](const std::unique_ptr<Sink>& sink) -> bool
		{
			return sink->GetRegisteredName() == sinkName;
		};

	const auto finded = std::find_if(m_globalSinks.begin(), m_globalSinks.end(), finder);

	if (finded == m_globalSinks.end())
		assert(false);

	auto targetSplit = Utils::SplitString(target, ".");

	if (targetSplit.size() != 2)
		assert(false);

	(*finded)->SetTarget(targetSplit[0], targetSplit[1]);
}

Pass* RenderGraph::findPassByNameOrNull(const std::string& name)
{
	for (auto& pass : m_passes)
	{
		if (pass->GetName() == name)
			return pass.get();
	}

	return nullptr;
}

void RenderGraph::linkSinks(Pass& pass)
{
	for (auto& sink : pass.GetSinks())
	{
		const auto& inputSourcePassName = sink->GetPassName();

		if (inputSourcePassName == "$")
		{
			bool bound = false;

			for (auto& source : m_globalSources)
			{
				if (source->GetName() == sink->GetOutputName())
				{
					sink->BindSink(*source);
					bound = true;

					break;
				}
			}

			if (!bound)
				assert(false);
		}
		else
		{
			bool bound = false;

			for (auto& pass : m_passes)
			{
				if (pass->GetName() == inputSourcePassName)
				{
					auto& source = pass->GetSource(sink->GetOutputName());
					sink->BindSink(source);
					bound = true;

					break;
				}
			}

			if (!bound)
				assert(false);
		}
	}
}

void RenderGraph::linkGlobalSinks()
{
	for (auto& sink : m_globalSinks)
	{
		const auto& inputSourcePassName = sink->GetPassName();

		for (auto& pass : m_passes)
		{
			if (pass->GetName() == inputSourcePassName)
			{
				auto& source = pass->GetSource(sink->GetOutputName());
				sink->BindSink(source);

				break;
			}
		}
	}
}
