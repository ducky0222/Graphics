#include "pch.h"
#include "BindingPass.h"

#include "RenderTarget.h"
#include "DepthStencil.h"

BindingPass::BindingPass(std::string name, std::vector<std::shared_ptr<IBindable>> binds /*= {}*/)
 : Pass(std::move(name)), m_binds(std::move(binds))
{

}

void BindingPass::AddBind(std::shared_ptr<IBindable> bind)
{
	m_binds.emplace_back(std::move(bind));
}

void BindingPass::BindAll(FlatGraphics& graphics) const
{
	bindBufferResources(graphics);

	for (auto& bind : m_binds)
		bind->Bind(graphics);
}

void BindingPass::Finalize()
{
	Pass::Finalize();
	assert(m_renderTarget || m_depthStencil);
}

void BindingPass::bindBufferResources(FlatGraphics& graphics) const
{
	if (m_renderTarget)
		m_renderTarget->BindBuffer(graphics, m_depthStencil.get());
	else
		m_depthStencil->BindBuffer(graphics);
}
