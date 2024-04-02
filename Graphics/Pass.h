#pragma once

#include "Sink.h"

class FlatGraphics;
class RenderTarget;
class DepthStencil;

class Pass
{
public:
	Pass(std::string name);
	virtual ~Pass() = default;

	virtual void OnResize(FlatGraphics& graphics, unsigned width, unsigned height) {}
	
	virtual void Execute(FlatGraphics& graphics) const abstract;
	virtual void Reset() {}
	virtual void Finalize();

	void SetSinkLinkage(const std::string& registerName, const std::string& target);

	Source& GetSource(const std::string& registerName) const;
	Sink& GetSink(const std::string& registerName) const;

	const std::vector<std::unique_ptr<Sink>>& GetSinks() const { return m_sinks; }
	const std::string& GetName() const { return m_name; }

protected:
	void registerSink(std::unique_ptr<Sink> sink);
	void registerSource(std::unique_ptr<Source> source);

private:
	std::vector<std::unique_ptr<Sink>> m_sinks;
	std::vector<std::unique_ptr<Source>> m_sources;
	std::string m_name;
};

