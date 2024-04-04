#include "pch.h"
#include "Job.h"

Job::Job(const Step* step, IDrawable* drawable)
	:m_drawable(drawable), m_step(step)
{

}

void Job::Execute(FlatGraphics& graphics, bool hasAlpha, size_t layer) const
{

}
