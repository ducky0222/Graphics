#include "pch.h"
#include "Job.h"
#include "Step.h"
#include "IDrawable.h"
#include "Mesh.h"
#include "Model.h"

Job::Job(const Step* step, IDrawable* drawable)
	:m_drawable(drawable), m_step(step)
{

}

void Job::Execute(FlatGraphics& graphics, bool hasAlpha, size_t layer) const
{
	m_drawable->Bind(graphics);

	switch (m_drawable->GetDrawableType())
	{
		case DrawableType::MESH:
		{
			if (layer != 0)
				return;

			Mesh* model = static_cast<Mesh*>(m_drawable);
			m_step->Bind(graphics);
			graphics.DrawIndexed(m_drawable->GetIndexCount());
			break;
		}
		default:
		{
			m_step->Bind(graphics);
			graphics.DrawIndexed(m_drawable->GetIndexCount());
			break;
		}
	}
}
