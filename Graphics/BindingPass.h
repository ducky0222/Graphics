#pragma once

#include "Pass.h"

class RenderTarget;
class DepthStencil;

class BindingPass : public Pass
{
protected:
	BindingPass(std::string name, std::vector<std::shared_ptr<IBindable>> binds = {});
	virtual ~BindingPass() = default;

	void AddBind(std::shared_ptr<IBindable> bind);
	void BindAll(FlatGraphics& graphics) const;
	void Finalize() override;

	template<typename T>
	void AddBindSink(std::string name);
	
private:
	void bindBufferResources(FlatGraphics& graphics) const;

protected:
	std::shared_ptr<RenderTarget> m_renderTarget = nullptr;
	std::shared_ptr<DepthStencil> m_depthStencil = nullptr;

private:
	std::vector<std::shared_ptr<IBindable>> m_binds;
};

template<typename T>
void BindingPass::AddBindSink(std::string name)
{
	const auto index = m_binds.size();
	m_binds.emplace_back();
	registerSink(std::make_unique<ContainerBindableSink<T>>(std::move(name), m_binds, index));
}

