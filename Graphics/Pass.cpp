#include "pch.h"
#include "Pass.h"
#include "Utils.h"

Pass::Pass(std::string name)
	: m_name(name)
{

}

void Pass::Finalize()
{
	// 싱크와 소스가 연결되었는지 확인
	for (auto& sink : m_sinks)
		sink->PostLinkValidate();

	for (auto& source : m_sources)
		source->PostLinkValidate();
}

void Pass::SetSinkLinkage(const std::string& registeredName, const std::string& target)
{
	auto& sink = GetSink(registeredName);

	std::vector<std::string> targetSplit = Utils::SplitString(target, ".");
	if (targetSplit.size() != 2)
		assert(false);

	sink.SetTarget(targetSplit[0], std::move(targetSplit[1]));
}

Source& Pass::GetSource(const std::string& name) const
{
	for (auto& source : m_sources)
	{
		if (source->GetName() == name)
			return *source;
	}

	assert(false);
	return *m_sources[0];
}

Sink& Pass::GetSink(const std::string& registeredName) const
{
	for (auto& sink : m_sinks)
	{
		if (sink->GetRegisteredName() == registeredName)
			return *sink;
	}

	assert(false);
	return *m_sinks[0];
}

void Pass::registerSink(std::unique_ptr<Sink> sink)
{
	// 하나의 랜더 그래프에 중복된 이름이 있으면 안됨
	for (auto& _sink : m_sinks)
	{
		if (_sink->GetRegisteredName() == sink->GetRegisteredName())
			assert(false);
	}

	m_sinks.emplace_back(std::move(sink));
}

void Pass::registerSource(std::unique_ptr<Source> source)
{
	// 하나의 랜더 그래프에 중복된 이름이 있으면 안됨
	for (auto& src : m_sources)
	{
		if (src->GetName() == source->GetName())
			assert(false);
	}

	m_sources.emplace_back(std::move(source));
}
